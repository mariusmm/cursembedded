/**
 ******************************************************************************
 * @file    BSP.h
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-July-2017
 * @brief   Basic BSP enabling printf
 * @license This project is released under the GNU Public License.s
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


void TriggerSet(void);
void TriggerClear(void);
void TriggerToggle(void);

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
