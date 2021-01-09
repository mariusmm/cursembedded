/*
 * ADPS9960.c
 *
 *  Created on: 20 de des. 2020
 *      Author: marius
 */

#include "em_i2c.h"
#include "em_cmu.h"
#include "em_gpio.h"

#include "RegisterFile.h"

#define DEVICE_ADDR (0x39 << 1)
#define APDS_ENABLE_REG (0x80)
#define APDS_CTRL_1_REG (0x8F)
#define APDS_CTRL_2_REG (0x90)
#define APDS_STATUS_REG (0x93)
#define APDS_PDATA_REG  (0x9C)

static bool I2C_WriteRegister(uint8_t reg, uint8_t data);
static bool I2C_ReadRegister(uint8_t reg, uint8_t *val);

bool ADPS9960_Init(void) {
	uint8_t ret_value;

	I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;

	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_I2C0, true);

	GPIO_PinModeSet(gpioPortD, 7, gpioModeWiredAnd, 1);
	GPIO_PinModeSet(gpioPortD, 6, gpioModeWiredAnd, 1);

	I2C0->ROUTE = I2C_ROUTE_SDAPEN |
	I2C_ROUTE_SCLPEN | I2C_ROUTE_LOCATION_LOC1;

	I2C_Init(I2C0, &i2cInit);

	I2C_ReadRegister(0x92, &ret_value);

	if (ret_value != 0xAB) {
		while (1);
	}

	/* Enable Proximity detection */
	/* ENABLE <- 5 & 2 & 0 bits */
	I2C_WriteRegister(APDS_ENABLE_REG, 0x25);

	/* LED Strength to 100mA, Proximity Gain control to 8x */
	I2C_WriteRegister(APDS_CTRL_1_REG, 0x0C);

	/* LED_BOOST 300% 0111_0001 */
	I2C_WriteRegister(APDS_CTRL_2_REG, 0x71);

	return true;
}

uint8_t ADPS9960_GetDistance(void) {
	uint8_t aux;
	uint8_t status;

	do {
		I2C_ReadRegister(APDS_STATUS_REG, &status);
	} while (status == 0x00);

	I2C_ReadRegister(APDS_PDATA_REG, &aux);
	return aux;
}

void APDS9960Distance_cb(int reg_num) {
	uint8_t aux;

	aux = ADPS9960_GetDistance();
	register_direct_write (REGISTER_DISTANCE, aux);
}

static bool I2C_WriteRegister(uint8_t reg, uint8_t data) {
	I2C_TransferReturn_TypeDef I2C_Status;
	bool ret_value = false;

	I2C_TransferSeq_TypeDef seq;
	uint8_t dataW[2];

	seq.addr = DEVICE_ADDR;
	seq.flags = I2C_FLAG_WRITE;

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
