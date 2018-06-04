/**
 ******************************************************************************
 * @file    APDS-9960.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    24-May-2018
 * @brief   APDS-9969 sensor library
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

#include "APDS-9960.h"


void APDS_9960_Init(void) {

	I2C_initialize();

	APDS_9960_Detect();
}

void APDS_9960_Detect(void) {
	uint8_t ret_value;

	I2C_ReadRegister(DEVICE_ADDR, APDS_ID_REG, &ret_value);

	if (ret_value == APDS_ID) {
		printf("Detected!\r\n");
	} else {
		printf("Something went wrong\r\n");
	}
}

void APDS_9960_InitProximity() {
	//Enable Proximity detection
	// ENABLE <- 5 & 2 & 0 bits
	I2C_WriteRegister(DEVICE_ADDR, APDS_ENABLE_REG, 0x25);

	/* LED Strength to 100mA, Proximity Gain control to 8x */
	I2C_WriteRegister(DEVICE_ADDR, APDS_CTRL_1_REG, 0x0C);

	/* LED_BOOST 300% 0111_0001*/
	I2C_WriteRegister(DEVICE_ADDR, APDS_CTRL_2_REG, 0x71);
}


bool APDS_9960_ReadProximity(uint8_t *p_data) {
	uint8_t status;
	bool ret = false;

	I2C_ReadRegister(DEVICE_ADDR, APDS_STATUS_REG, &status);

	if ((status & 0x02) != 0x00) {
		I2C_ReadRegister(DEVICE_ADDR, APDS_PDATA_REG, p_data);
		ret = true;
	}

	return ret;
}
