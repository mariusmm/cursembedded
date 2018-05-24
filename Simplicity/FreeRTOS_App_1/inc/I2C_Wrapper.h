/**
 ******************************************************************************
 * @file    I2C_Wrapper.h
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    23-May-2018
 * @brief   APDS-9969 library
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

/** @addtogroup I2C_Wrapper
 * @brief I2C_Wrapper Wrapper for FreeRTOS for emlib I2C library
 * @{
 */

#ifndef INC_I2C_WRAPPER_H_
#define INC_I2C_WRAPPER_H_

#include "em_emu.h"
#include "em_i2c.h"

#include "FreeRTOS.h"
#include "semphr.h"

#include "BSP.h"

/**
 * @brief Initializes I2C subsystem
 */
void I2C_initialize(void);

/**
 * @brief Writes an I2C register
 * @param[in] addr I2C address to access
 * @param[in] reg Register address to access
 * @param[in] data data to write
 * @return true if successful
 */
bool I2C_WriteRegister(uint8_t addr, uint8_t reg, uint8_t data);

/**
 * @brief Reads an I2C register
 * @param[in] addr I2C address to access
 * @param[in] reg Register address to access
 * @param[out] val data read
 * @return true if successful
 */
bool I2C_ReadRegister(uint8_t addr, uint8_t reg, uint8_t *val);

#endif /* INC_I2C_WRAPPER_H_ */

/**
 * @}
 */
