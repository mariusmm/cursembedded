/**
 ******************************************************************************
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @file    PeriodicTasks.h
 * @version V1.0
 * @date    16-January-2019
 * @brief   Periodic tasks example with a simple kernel
 * @license This project is released under the GNU Public License
 *
 *
 * ******************************************************************************
 */

#ifndef INC_PERIODICTASKS_H_
#define INC_PERIODICTASKS_H_

#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_rtc.h"

#include "rtcdriver.h"
#include "sleep.h"

typedef void (*mycallback_t)(void);

/**
 * @brief initialization function for library PeriodicTasks
 */
void Tasks_Init();


/**
 * @brief Registers new task
 * @param func pointer to task function
 * @param period period time in milliseconds
 */
void RegisterTask(mycallback_t func, uint32_t period);

/**
 * @brief Executes kernel
 */
void KernelExec();


#endif /* INC_PERIODICTASKS_H_ */
