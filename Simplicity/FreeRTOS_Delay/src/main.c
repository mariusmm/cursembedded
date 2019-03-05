/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-March-2019
 * @brief   DelayUntil FreeRTOS example
 * @license This project is released under the GNU Public License.
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

/* emlib includes */
#include "em_chip.h"

/* emdrv includes */
#include "sleep.h"

/* our includes */
#include "BSP.h"

/** Priority for the ToogleLed Task*/
#define TOGGLE_TASK_PRIORITY          (tskIDLE_PRIORITY + 1)

/**
 * @brief Our first task. A task is always a endless loop
 *
 * @param pParameter Unused parameter
 * @return none
 *
 */
static void TaskLedToggle(void *pParameter) {

	(void) pParameter;
	TickType_t previous_tick_time;
	TickType_t delay_time;
	uint32_t random_time;

	previous_tick_time = xTaskGetTickCount();

	printf("Tick\tDelay\r\n");
	printf("***************\r\n");

	for (;;) {
		LedToggle();
		printf("%lu", xTaskGetTickCount());

		/* Random delay from o to 300 ms. */
		random_time = rand() % 300;
		printf("\t %lu\r\n", random_time);
		delay_time = pdMS_TO_TICKS(random_time);
		vTaskDelay(delay_time);

		/* We want the task exactly every 500 milliseconds independently
		 * from the (different) execution time */
		vTaskDelayUntil(&previous_tick_time, pdMS_TO_TICKS(500));
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

	/* Create our first task */
	xTaskCreate(TaskLedToggle, (const char *) "LedToggle",
			configMINIMAL_STACK_SIZE, NULL, TOGGLE_TASK_PRIORITY, NULL);

	/* Start FreeRTOS Scheduler */
	vTaskStartScheduler();

	return 0;
}

/*
 * @}
 */

/**
 * @mainpage FreeRTOS basic example
 *
 * This example shows a basic example using DelayUntil function.
 *
 * It creates a single task that toggles the LED present in the prototyping board every 500 milliseconds.
 * Also, every execution has a random delay time to show how the function DelayUntil manages these variations.
 *
 * This project uses the following libraries:
 * @li @ref BSP
 *
 * @}
 */
