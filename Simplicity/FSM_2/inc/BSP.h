/**
 ******************************************************************************
 * @file    BSP.h
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    09-January-2019
 * @brief   Basic BSP enabling pritnf
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */
#ifndef BSP_H_
#define BSP_H_

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"

void setupSWOForPrint(void);

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


#endif /* BSP_H_ */
