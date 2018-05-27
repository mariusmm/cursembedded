/**
 ******************************************************************************
 * @file    TraceUtils.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    25-May-2018
 * @brief   Trace utilities for FreeRTOS
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

#include "TraceUtils.h"

void Trace_tasks(void *pParameter) {

	int num_tasks, i;

	while (1) {
		vTaskDelay(5000);
		num_tasks = uxTaskGetNumberOfTasks();

		printf("num of tasks %d\r\n", num_tasks);

		for (i = 0; i < MAX_TASKS_TO_TRACE; i++) {
			printf("Task %s - Watermark:%lu\r\n",
					pcTaskGetName(tasks_handles[i]),
					uxTaskGetStackHighWaterMark(tasks_handles[i]));
		}
	}
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName) {
	printf("Task %s problem: Stack Overflow!\r\n", pcTaskName);
}
