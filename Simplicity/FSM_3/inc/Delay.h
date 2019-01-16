/**
 ******************************************************************************
 * @file    Delay.h
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    15-January-2019
 * @brief	Delay functions
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_emu.h"
#include "em_int.h"

#include "rtcdriver.h"

/**
 * @brief initializtion function for the library
 */
void Delay_Init();

/**
 * @brief Basic delay function
 * @param ms time in milliseconds
 */
void Delay(int ms);

/**
 * @brief Delay function that sets the CPU in EM1
 * @param ms time in milliseconds
 */
void EM1_Delay(int ms);

/**
 * @brief Delay function that sets the CPU in EM2
 * @param ms time in milliseconds
 */
void EM2_Delay(int ms);

#endif /* INC_DELAY_H_ */
