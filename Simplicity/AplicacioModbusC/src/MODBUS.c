/*
 * MODBUS.c
 *
 *  Created on: 19 de des. 2020
 *      Author: marius
 */

#include <string.h>

#include "em_device.h"
#include "em_cmu.h"
#include "em_leuart.h"

#include "definitions.h"
#include "SystemDelay.h"
#include "RegisterFile.h"

static int modbus_do_func3(uint8_t *pkt);
static int modbus_do_func6(uint8_t *pkt);
static uint16_t CRC16(uint8_t *buf, int len);

void modbus_init(void) {

	LEUART_Init_TypeDef leuart_cfg = LEUART_INIT_DEFAULT;

	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_CORELE, true);
	CMU_ClockEnable(cmuClock_LEUART0, true);

	GPIO_PinModeSet(gpioPortD, 4, gpioModePushPull, 1); /* TX Pin */
	GPIO_PinModeSet(gpioPortD, 5, gpioModeInputPull, 1); /* RX Pin */

	LEUART_Init(LEUART0, &leuart_cfg);
	LEUART0->ROUTE = LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN
			| LEUART_ROUTE_LOCATION_LOC0;

	LEUART_IntEnable(LEUART0, LEUART_IEN_RXDATAV);
	NVIC_ClearPendingIRQ(LEUART0_IRQn);
	NVIC_EnableIRQ(LEUART0_IRQn);
}

int16_t modbus_RxTimeout() {
	bool data = false;
	int16_t c = -1;
	int retries = 0;

	do {
		if ((LEUART0->STATUS & LEUART_STATUS_RXDATAV) != 0) {
			data = true;
			c = (uint16_t) LEUART0->RXDATA;
			break;
		}
		retries++;
		SysTicksDelay(1);
	} while ((data == false) && (retries < 50));

	return c;
}

void modbus_send(const uint8_t *const pkt, int len) {

	for (int i = 0; i < len; i++) {
		LEUART_Tx(LEUART0, pkt[i]);
	}
}

int modbus_slave(void) {
	uint8_t modbus_pkt[MODBUS_SLAVE_BUFFER_SIZE] = { 0 };
	int retries = 0;
	int idx_wr = 0;
	int16_t inchar = 0;
	int ret_val = 0;

	do {
		inchar = modbus_RxTimeout();
		if (inchar != -1) {
			modbus_pkt[idx_wr++] = inchar;
		} else {
			retries++;
			SysTicksDelay(5);
		}
	} while ((retries < 5) && (idx_wr < MODBUS_SLAVE_BUFFER_SIZE));

	uint8_t modbus_addr;
	uint8_t modbus_func;

	modbus_addr = modbus_pkt[0];
	modbus_func = modbus_pkt[1];

	if (modbus_addr != MODBUS_SLAVE_ADDRRESS) {
		ret_val = -1;
	} else {
		switch (modbus_func) {
		case 3:
			ret_val = modbus_do_func3(modbus_pkt);
			break;
		case 6:
			ret_val = modbus_do_func6(modbus_pkt);
			break;
		default:
			ret_val = -1;
		}
	}

	if (ret_val > 0) {
		register_store();
	}

	LEUART_IntEnable(LEUART0, LEUART_IEN_RXDATAV);

	return ret_val;
}

int modbus_do_func3(uint8_t *pkt) {
	uint16_t reg_addr = 0;
	uint16_t reg_qty = 0;
	uint16_t pkt_crc = 0;
	uint16_t calc_crc = 0;
	uint8_t modbus_resp_pkt[MODBUS_SLAVE_BUFFER_SIZE];
	int wr_idx = 0;

	reg_addr = pkt[2] << 8;
	reg_addr |= pkt[3] & 0x00FF;

	reg_qty = pkt[4] << 8;
	reg_qty |= pkt[5] & 0x00FF;

	pkt_crc = pkt[7] << 8 | pkt[6];
	calc_crc = CRC16(pkt, 6);
	if (pkt_crc != calc_crc) {
		return -1;
	}

	modbus_resp_pkt[wr_idx++] = MODBUS_SLAVE_ADDRRESS;
	modbus_resp_pkt[wr_idx++] = 3;
	modbus_resp_pkt[wr_idx++] = reg_qty * 2;

	for (unsigned int i = 0; i < reg_qty; i++) {
		uint16_t value = register_read(i + reg_addr);
		modbus_resp_pkt[wr_idx++] = (value & 0xFF00) >> 8;
		modbus_resp_pkt[wr_idx++] = value & 0x00FF;
	}

	uint16_t resp_pkt_crc;
	resp_pkt_crc = CRC16(modbus_resp_pkt, wr_idx);
	modbus_resp_pkt[wr_idx++] = resp_pkt_crc & 0x00FF;
	modbus_resp_pkt[wr_idx++] = (resp_pkt_crc & 0xFF00) >> 8;

	modbus_send(modbus_resp_pkt, wr_idx);

	return 0;
}

int modbus_do_func6(uint8_t *pkt) {
	uint16_t reg_addr = 0;
	uint16_t reg_value = 0;
	uint16_t pkt_crc = 0;
	uint16_t calc_crc = 0;

	reg_addr = pkt[2] << 8;
	reg_addr |= pkt[3] & 0x00FF;

	reg_value = pkt[4] << 8;
	reg_value |= pkt[5] & 0x00FF;

	pkt_crc = pkt[7] << 8 | pkt[6];
	calc_crc = CRC16((uint8_t*) pkt, 6);

	if (pkt_crc != calc_crc) {
		return -1;
	}

	bool permanent = register_write(reg_addr, reg_value);

	modbus_send(pkt, 8);

	if (permanent == true) {
		return 1;
	} else {
		return 0;
	}
}

uint16_t CRC16(uint8_t *buf, int len) {
	uint16_t crc = 0xFFFF;

	for (int pos = 0; pos < len; pos++) {
		crc ^= (uint16_t) buf[pos];

		for (int i = 8; i != 0; i--) {
			if ((crc & 0x0001) != 0) {
				crc >>= 1;
				crc ^= 0xA001;
			} else
				crc >>= 1;
		}
	}

	return crc;
}

void LEUART0_IRQHandler(void) {
	uint32_t flags;
	flags = LEUART_IntGet(LEUART0);
	LEUART_IntClear(LEUART0, flags);
	LEUART_IntDisable(LEUART0, LEUART_IEN_RXDATAV);
}
