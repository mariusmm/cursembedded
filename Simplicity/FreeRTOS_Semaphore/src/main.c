/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    25-July-2017
 * @brief   Basic FreeRTOS Semaphore example
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

/** Priority for the ToogleLed Task*/
#define TOGGLE_TASK_PRIORITY          (tskIDLE_PRIORITY + 1)

/** @brief binary semaphore for button 0 */
SemaphoreHandle_t semaphore_button_0;

/**
 * @brief Button 0 ISR
 *
 * Gives corresponding binary semaphore every time it enters
 */
void GPIO_EVEN_IRQHandler(void) {
	uint32_t aux;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	/* clear flags */
	aux = GPIO_IntGet();
	GPIO_IntClear(aux);

	/* Toggle semaphore */
	xSemaphoreGiveFromISR(semaphore_button_0, &xHigherPriorityTaskWoken);

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * @brief Button 1 ISR
 *
 *
 */
void GPIO_ODD_IRQHandler(void) {
	uint32_t aux;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	/* clear flags */
	aux = GPIO_IntGet();
	GPIO_IntClear(aux);

	/* nothing to do */
}

/**
 * @brief Our first task. A task is always a endless loop
 *
 * @param pParameter Unused parameter
 * @return none
 *
 */
static void TaskLedToggle(void *pParameter) {

	(void) pParameter;

	/* Here optional init variables or functions */

	for (;;) {
		xSemaphoreTake(semaphore_button_0, portMAX_DELAY);
		LedToggle();
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

	/* Create semaphores */
	semaphore_button_0 = xSemaphoreCreateBinary();

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
 * @mainpage FreeRTOS basic semaphore example
 *
 *
 * This examples uses a binary semaphore to signal from a ISR to a task
 *
 * The ISR is attached to button 0 IRQ and "gives" a binary semaphore
 *
 * The task is trying to "take" the binary semaphore and when it does,
 * just toggles the led
 *
 *
 * This project uses the following libraries:
 * @li @ref BSP
 *
 * @}
 */
