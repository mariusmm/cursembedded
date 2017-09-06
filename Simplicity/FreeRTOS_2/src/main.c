/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    27-July-2017
 * @brief   More second FreeRTOS example
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
#include "queue.h"
#include "semphr.h"

/* emlib includes */
#include "em_chip.h"

/* emdrv includes */
#include "sleep.h"

/* our includes */
#include "BSP.h"

/** MAximum delay time */
#define MAX_DELAY pdMS_TO_TICKS(2000)

/** DELAY INCREMENT */
#define DELAY_DELTA pdMS_TO_TICKS(250)

/** Priority for the ToogleLed Task*/
#define TOGGLE_TASK_PRIORITY          (tskIDLE_PRIORITY + 1)

/** Legnth of the queue (10 items) */
#define QUEUE_LENGTH (10)

/** @brief Queue for buttons */
QueueHandle_t queue_tasks;

/** @brief Binary semaphore to signal @ref TaskCalculateDelay from ISR*/
SemaphoreHandle_t semaphore_button_0;

/**
 * @brief Button 0 ISR
 *
 * Signals task @ref TaskCalculateDelay every time button 0 is pushed
 */
void GPIO_EVEN_IRQHandler(void) {
	uint32_t aux;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	/* clear flags */
	aux = GPIO_IntGet();
	GPIO_IntClear(aux);

	xSemaphoreGiveFromISR(semaphore_button_0, &xHigherPriorityTaskWoken);

	/* Awake a task ? */
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * @brief Button 1 ISR
 *
 * Unused
 */
void GPIO_ODD_IRQHandler(void) {
	uint32_t aux;

	/* clear flags */
	aux = GPIO_IntGet();
	GPIO_IntClear(aux);
}

/**
 * @brief First task, calculates new delay every time button 0 is pressed
 *
 * This task receives signal from ISR through semaphore
 * @ref semaphore_button_0 and calculates new delay time for task
 * @ref TaskLedToggle. The new data is sent using queue @ref
 * queue_tasks.
 *
 * @param pParameter Unused parameter
 * @return none
 *
 */
static void TaskCalculateDelay(void *pParameter) {

	(void) pParameter;
	uint32_t delay;

	/* Here optional init variables or functions */

	delay = pdMS_TO_TICKS(250);

	for (;;) {

		xSemaphoreTake(semaphore_button_0, portMAX_DELAY);

		delay += DELAY_DELTA;
		if (delay > MAX_DELAY) {
			delay = pdMS_TO_TICKS(250);
		}

		/* try to get new delay time from queue */
		(xQueueSend(queue_tasks, &delay, (TickType_t ) 0));

	}
}

/**
 * @brief Second tasks receives delay and toggles LED using
 * this delay
 *
 * This tasks tries to receive from queue @ref queue_tasks, if so, gets
 * the new delay time and applies it to toggling LED.
 *
 * @param pParameter Unused parameter
 * @return none
 *
 */
static void TaskLedToggle(void *pParameter) {
	(void) pParameter;
	uint32_t my_delay;
	uint32_t recv_delay;

	my_delay = pdMS_TO_TICKS(250);

	while (true) {
		if (xQueueReceive(queue_tasks, &recv_delay, (TickType_t ) 0)) {
			my_delay = recv_delay;
		}

		/* wait for m_delay & toggle the LED */
		vTaskDelay(my_delay);
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

	/* Create Queue */
	queue_tasks = xQueueCreate(QUEUE_LENGTH, sizeof(uint32_t));

	/* Create Semaphore */
	semaphore_button_0 = xSemaphoreCreateBinary();

	/* Create tasks */

	xTaskCreate(TaskCalculateDelay, (const char *) "CalculateDelay",
	configMINIMAL_STACK_SIZE, NULL, TOGGLE_TASK_PRIORITY, NULL);

	xTaskCreate(TaskLedToggle, (const char *) "LedToggle",
	configMINIMAL_STACK_SIZE, NULL, TOGGLE_TASK_PRIORITY, NULL);

	/* Start FreeRTOS Scheduler */
	vTaskStartScheduler();

	return 0;
}

/**
 * @}
 */

