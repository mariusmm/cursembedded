/**
 ******************************************************************************
 * @file    PWM.h
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    24-May-2018
 * @brief   Basic PWM library
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

/** @addtogroup PWM
 * @brief PWM library
 * @{
 */


#ifndef INC_PWM_H_
#define INC_PWM_H_

#include <stdio.h>

#include "em_timer.h"

#include "BSP.h"

/**
 * PD7 (Led) is connected to TIM1_CC1 #4
 *
 * PWM Frequency is : 14_000_000 / 256 / 4000 = 13,67 Hz
 *
 */

#define PWM_FREQ 4096

/**
 * Initialize PWM library
 */
void PWM_Init(void);

/**
 * Set PWM duty cycle %
 * @param percentage % of duty cycle to set
 */
void PWM_Set(uint8_t percentage);


#endif /* INC_PWM_H_ */

/**
 * @}
 */
