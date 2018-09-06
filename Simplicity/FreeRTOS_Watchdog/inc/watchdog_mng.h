/**
 ******************************************************************************
 * @file    watchdog_mng.h
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-September-2018
 * @brief   Watchdog manager for FreeRTOS
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

/** @addtogroup Watchdoglib
 * @brief Watchdog library
 * @{
 */

#ifndef INC_WATCHDOG_MNG_H_
#define INC_WATCHDOG_MNG_H_

#include "em_wdog.h"
#include "em_cmu.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define WATCHDOG_TASK1 0x01
#define WATCHDOG_TASK2 0x02

//#define WATCHDOG_FULL (WATCHDOG_TASK1 | WATCHDOG_TASK2)
#define WATCHDOG_FULL (WATCHDOG_TASK1)

#define WDT_TASK_PRIORITY (tskIDLE_PRIORITY + 1)

/**
 * @brief
 * @param task ID of the task
 */
void watchdogTouch(uint8_t task);

/**
 * Initialize watchdog library
 * @return true if everything OK, false otherwise
 */
bool watchdogInit();



#endif /* INC_WATCHDOG_MNG_H_ */

/**
 * @}
 */
