/**
 ******************************************************************************
 * @file    I2C_Example_1.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    08-April-2018
 * @brief   I2C Example 1
 * @license This project is released under the GNU Public License.
 *
 * I2C Pins are I2C0 #1:
 *    - SCL : PD7 -> Pin 15
 *    - SDA: PD6 -> Pin 16
 *
 * APDS-9960 RGB/Gesture -> I2C Addr: 0x39, Info Reg. 0x92 = 0xAB
 *
 * ******************************************************************************
 */

#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_usart.h"
#include "em_emu.h"
#include "em_i2c.h"

#include "BSP.h"
#include <stdio.h>

#define DEVICE_ADDR (0x39 << 1)
;
USART_InitAsync_TypeDef usart_cfg = USART_INITASYNC_DEFAULT;

static bool I2C_WriteRegister(uint8_t reg, uint8_t data) {
	I2C_TransferReturn_TypeDef I2C_Status;
	bool ret_value = false;

	I2C_TransferSeq_TypeDef seq;
	uint8_t dataW[2];

	seq.addr = DEVICE_ADDR;
	seq.flags = I2C_FLAG_WRITE;

	/* Register to write: 0x67 ( INT_FLAT )*/
	dataW[0] = reg;
	dataW[1] = data;

	seq.buf[0].data = dataW;
	seq.buf[0].len = 2;
	I2C_Status = I2C_TransferInit(I2C0, &seq);

	while (I2C_Status == i2cTransferInProgress) {
		I2C_Status = I2C_Transfer(I2C0);
	}

	if (I2C_Status != i2cTransferDone) {
		ret_value = false;
	} else {
		ret_value = true;
	}
	return ret_value;
}

static bool I2C_ReadRegister(uint8_t reg, uint8_t *val) {
	I2C_TransferReturn_TypeDef I2C_Status;
	I2C_TransferSeq_TypeDef seq;
	uint8_t data[2];

	seq.addr = DEVICE_ADDR;
	seq.flags = I2C_FLAG_WRITE_READ;

	seq.buf[0].data = &reg;
	seq.buf[0].len = 1;
	seq.buf[1].data = data;
	seq.buf[1].len = 1;

	I2C_Status = I2C_TransferInit(I2C0, &seq);

	while (I2C_Status == i2cTransferInProgress) {
		I2C_Status = I2C_Transfer(I2C0);
	}

	if (I2C_Status != i2cTransferDone) {
		return false;
	}

	*val = data[0];

	return true;
}

void testI2C(void) {
	uint8_t ret_value;

	I2C_ReadRegister(0x92, &ret_value);

	if (ret_value == 0xAB) {
		printf("Detected!\r\n");
	} else {
		printf("Something went wrong\r\n");
	}
}

int main(void) {
	/* Chip errata */
	CHIP_Init();

	setupSWOForPrint();

	CMU_ClockEnable(cmuClock_GPIO, true);

	/* I2C Part */
	I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;

	CMU_ClockEnable(cmuClock_I2C0, true);

	GPIO_PinModeSet(gpioPortD, 7, gpioModeWiredAnd, 1);
	GPIO_PinModeSet(gpioPortD, 6, gpioModeWiredAnd, 1);

	/* Enable pins at location 3 (which is used on the DVK) */
	I2C0->ROUTE = I2C_ROUTE_SDAPEN |
			I2C_ROUTE_SCLPEN | I2C_ROUTE_LOCATION_LOC1;

	I2C_Init(I2C0, &i2cInit);

	testI2C();

	/* Infinite loop */
	while (1) {
		;
	}
}
