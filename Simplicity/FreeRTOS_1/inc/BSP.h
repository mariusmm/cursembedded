/**
 ******************************************************************************
 * @file    BSP.h
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-July-2017
 * @brief   Basic BSP enabling printf
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
 */

/** @addtogroup BSP
 * @brief BSP library
 * @{
 */

#ifndef BSP_H_
#define BSP_H_

#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_timer.h"
#include "em_adc.h"

#include "sleep.h"

/**
 * PD7 (Led) is connected to TIM1_CC1 #4
 *
 * PWM Frequency is : 14_000_000 / 256 / 4000 = 13,67 Hz
 *
 */

#define PWM_FREQ 4000

/**
 * @brief Init all BSP functions
 */
void BSP_Init(void);

/**
 * @brief Switch on LED
 */
void LedOn(void);

/**
 * @brief Switch off LED
 */
void LedOff(void);

/**
 * @brief Switch off LED
 */
void LedToggle(void);

/**
 * @brief PWM (Timer) init
 */
void PWMConfig(void);

/**
 * @brief ADC init
 *
 *  PD6 --> ADC0_CH6 input
 */
void ADCConfig(void);

#endif /* BSP_H_ */

/**
 * @}
 */


/**
 * @addtogroup BSP
 * @{
 *
 * Example code using BSP
 * @code

 main() {
  ...
  BSP_Init();
  ...
  LedOn();
  ...
  LedOff();
 }

 * @endcode
 *
 * @}
 */
