/**
 ******************************************************************************
 * @file    TraceUtils.h
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    25-May-2018
 * @brief   Trace utilities for FreeRTOS
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

/** @addtogroup Trace
 * @brief Trace library
 * @{
 */

#ifndef INC_TRACEUTILS_H_
#define INC_TRACEUTILS_H_

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

/** @brief number of tasks to trace */
#define MAX_TASKS_TO_TRACE 3

/** @brief array to store the handles to tasks */
TaskHandle_t tasks_handles[MAX_TASKS_TO_TRACE];

/**
 * @brief Prints by the debug console the name
 * of the tasks and its maximum watermark level
 */
void trace_watermark();

/**
 * @brief Task to get trace information for all task of the system
 * every 5 seconds
 *
 * @param pParameter Unused parameter, declared due to FreeRTOS API
 */
void Trace_tasks(void *pParameter);

#endif /* INC_TRACEUTILS_H_ */

/**
 * @}
 */
