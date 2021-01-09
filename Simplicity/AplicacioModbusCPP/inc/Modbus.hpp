/*
 * modbus.hpp
 *
 *  Created on: 29 de des. 2020
 *      Author: marius
 */

#ifndef INC_MODBUS_HPP_
#define INC_MODBUS_HPP_

#include "UART.hpp"
#include "Register.hpp"
#include "SystemDelay.hpp"

class Modbus {
public:
	Modbus (SerialUART *p_uart, Register *p_reg) : m_uart(p_uart),
			m_reg(p_reg) { }

	int Slave() {
		uint8_t modbus_pkt[MODBUS_SLAVE_BUFFER_SIZE] = { 0 };
		int retries = 0;
		int idx_wr = 0;
		int16_t inchar = 0;
		int ret_val = 0;

		do {
			inchar = m_uart->RxTimeout();
			if (inchar != -1) {
				modbus_pkt[idx_wr++] = inchar;
			} else {
				retries++;
				SysTick_mng.Delay(10);
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
				ret_val = this->do_func3(modbus_pkt);
				break;
			case 6:
				ret_val = this->do_func6(modbus_pkt);
				break;
			default:
				ret_val = -1;
			}
		}

		if (ret_val > 0) {
			m_reg->store();
		}

		LEUART_IntEnable(LEUART0, LEUART_IEN_RXDATAV);

		return ret_val;
	}
private:

	int do_func3(uint8_t *pkt) {
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
			uint16_t value = m_reg->read(i + reg_addr);
			modbus_resp_pkt[wr_idx++] = (value & 0xFF00) >> 8;
			modbus_resp_pkt[wr_idx++] = value & 0x00FF;
		}

		uint16_t resp_pkt_crc;
		resp_pkt_crc = CRC16(modbus_resp_pkt, wr_idx);
		modbus_resp_pkt[wr_idx++] = resp_pkt_crc & 0x00FF;
		modbus_resp_pkt[wr_idx++] = (resp_pkt_crc & 0xFF00) >> 8;

		m_uart->send(modbus_resp_pkt, wr_idx);

		return 0;
	}

	int do_func6(uint8_t *pkt) {
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

		bool permanent = m_reg->write(reg_addr, reg_value);

		m_uart->send(pkt, 8);

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

	SerialUART *m_uart;
	Register *m_reg;
};

#endif /* INC_MODBUS_HPP_ */
