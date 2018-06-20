/**
 ******************************************************************************
 * @file    APDS-9960.h
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    24-May-2018
 * @brief   APDS-9969 sensor library
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

/** @addtogroup APDS_9960
 * @brief APDS_9960 sensor library
 * @{
 */

#ifndef INC_APDS_9960_H_
#define INC_APDS_9960_H_

#include <stdio.h>

#include "I2C_Wrapper.h"
#include "BSP.h"

/** @brief I2C address for APDS-9960 */
#define DEVICE_ADDR (0x39 << 1)

/** @brief Read-only ID register value */
#define APDS_ID 0xAB

#define APDS_ENABLE_REG (0x80)
#define APDS_CTRL_1_REG (0x8F)
#define APDS_CTRL_2_REG (0x90)
#define APDS_ID_REG		(0x92)
#define APDS_STATUS_REG (0x93)
#define APDS_PDATA_REG  (0x9C)

/**
 * @brief Initalizes ADPS_9960 library. Must be called prior any other call to this library
 */
void APDS_9960_Init(void);

/**
 * @brief Detects & checks APDS-9960 RGB/Gesture sensor
 */
void APDS_9960_Detect(void);

/**
 * @brief Configures the sensor to work as a proximity sensor
 */
void APDS_9960_InitProximity();

/**
 * @brief Reads proximity data and returns it.
 * It waits for data to be valid before read it.
 *
 * @param[out] p_data proximity data read
 * @return true if successfully read data, false if something was wrong
 */
bool APDS_9960_ReadProximity(uint8_t *p_data);

#endif /* INC_APDS_9960_H_ */


/**
 * @}
 */
