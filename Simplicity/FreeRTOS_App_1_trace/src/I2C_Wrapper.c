/*
 * I2C_Wrapper.c
 *
 *  Created on: 23 maig 2018
 *      Author: marius
 */

#include "I2C_Wrapper.h"

/** @brief mutex to be used by I2C wrapper library */
static SemaphoreHandle_t I2C_mutex;

void I2C_initialize() {
	I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;

	CMU_ClockEnable(cmuClock_I2C0, true);

	GPIO_PinModeSet(gpioPortD, 7, gpioModeWiredAnd, 1);
	GPIO_PinModeSet(gpioPortD, 6, gpioModeWiredAnd, 1);

	/* Enable pins at location 3 (which is used on the DVK) */
	I2C0->ROUTE = I2C_ROUTE_SDAPEN |
	I2C_ROUTE_SCLPEN | I2C_ROUTE_LOCATION_LOC1;

	I2C_Init(I2C0, &i2cInit);

	I2C_mutex = xSemaphoreCreateMutex();
}


bool I2C_WriteRegister(uint8_t addr, uint8_t reg, uint8_t data) {
	I2C_TransferReturn_TypeDef I2C_Status;
	bool ret_value = false;

	I2C_TransferSeq_TypeDef seq;
	uint8_t dataW[2];

	seq.addr = addr;
	seq.flags = I2C_FLAG_WRITE;

	/* Register to write */
	dataW[0] = reg;
	dataW[1] = data;

	seq.buf[0].data = dataW;
	seq.buf[0].len = 2;

	xSemaphoreTake(I2C_mutex, portMAX_DELAY);

	I2C_Status = I2C_TransferInit(I2C0, &seq);

	while (I2C_Status == i2cTransferInProgress) {
		I2C_Status = I2C_Transfer(I2C0);
	}

	xSemaphoreGive(I2C_mutex);

	if (I2C_Status != i2cTransferDone) {
		ret_value = false;
	} else {
		ret_value = true;
	}
	return ret_value;
}

bool I2C_ReadRegister(uint8_t addr, uint8_t reg, uint8_t *val) {
	I2C_TransferReturn_TypeDef I2C_Status;
	I2C_TransferSeq_TypeDef seq;
	uint8_t data[2];

	seq.addr = addr;
	seq.flags = I2C_FLAG_WRITE_READ;

	seq.buf[0].data = &reg;
	seq.buf[0].len = 1;
	seq.buf[1].data = data;
	seq.buf[1].len = 1;

	xSemaphoreTake(I2C_mutex, portMAX_DELAY);

	I2C_Status = I2C_TransferInit(I2C0, &seq);

	while (I2C_Status == i2cTransferInProgress) {
		I2C_Status = I2C_Transfer(I2C0);
	}

	xSemaphoreGive(I2C_mutex);

	if (I2C_Status != i2cTransferDone) {
		return false;
	}

	*val = data[0];

	return true;
}
