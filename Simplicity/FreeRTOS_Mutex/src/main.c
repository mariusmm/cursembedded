/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    25-July-2017
 * @brief   Basic FreeRTOS Mutex example
 * @license This project is released under the GNU Public License.
 *
 * ******************************************************************************
 */

/** @addtogroup main
 * @brief main file
 * @{
 */

/* STD libraries*/
#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS includes */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* emlib includes */
#include "em_chip.h"

/* emdrv includes */
#include "sleep.h"

/* our includes */
#include "BSP.h"

/** Priority for the tasks */
#define TASK_1_PRIORITY          (tskIDLE_PRIORITY + 1)
#define TASK_2_PRIORITY          (tskIDLE_PRIORITY + 1)

/** Select to use MUTEX */
//#define USE_MUTEX

SemaphoreHandle_t example_mutex;

/**
 * @brief Just print a string using two printfs, yielding in between
 *
 * @param pParameter Unused parameter, declared due to FreeRTOS API
 * @return none
 *
 */
static void Task1(void *pParameter) {

	(void) pParameter;

	while (pdTRUE) {
#ifdef USE_MUTEX
		if (xSemaphoreTake(example_mutex, portMAX_DELAY)) {
#else
		{
#endif
			LedToggle();
			printf("Some text ");
			taskYIELD();
			printf("from Task1\r\n");
			xSemaphoreGive(example_mutex);
			vTaskDelay(100 / portTICK_PERIOD_MS);

		}
	}
}

/**
 * @brief Just print a string using two printfs, yielding in between
 *
 * @param pParameter Unused parameter, declared due to FreeRTOS API
 * @return none
 *
 */
static void Task2(void *pParameter) {

	(void) pParameter;

	while (pdTRUE) {
#ifdef USE_MUTEX
		if (xSemaphoreTake(example_mutex, portMAX_DELAY)) {
#else
		{
#endif
			LedToggle();
			printf("Other text ");
			taskYIELD();
			printf("from Task 2\r\n");
			xSemaphoreGive(example_mutex);
			vTaskDelay(100 / portTICK_PERIOD_MS);
		}
	}
}

/**
 * @brief main function
 */
int main(void) {
	/* Chip errata */
	CHIP_Init();

	/* Init functions for our BSP */
	BSP_Init();

	/* Init SLEEP library */
	SLEEP_Init(NULL, NULL);
#if (configSLEEP_MODE < 3)
	SLEEP_SleepBlockBegin((SLEEP_EnergyMode_t) (configSLEEP_MODE + 1));
#endif

	/* Create mutex */
	example_mutex = xSemaphoreCreateMutex();

	/* Create our first task */
	xTaskCreate(Task1, (const char *) "Task_1",
	configMINIMAL_STACK_SIZE, NULL, TASK_1_PRIORITY, NULL);

	xTaskCreate(Task2, (const char *) "Task_2",
	configMINIMAL_STACK_SIZE, NULL, TASK_2_PRIORITY, NULL);

	/* Start FreeRTOS Scheduler */
	vTaskStartScheduler();

	return 0;
}

/**
 * @}
 */

/**
 * @mainpage FreeRTOS basic mutex example
 *
 *
 * This project uses the following libraries:
 * @li @ref BSP
 *
 * @}
 */
