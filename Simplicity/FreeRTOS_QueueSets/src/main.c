/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    25-April-2019
 * @brief   Basic FreeRTOS Queue set example
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

/* emlib includes */
#include "em_chip.h"

/* emdrv includes */
#include "sleep.h"

/* our includes */
#include "BSP.h"

/** Priority for the ToogleLed Task*/
#define TOGGLE_TASK_PRIORITY          (tskIDLE_PRIORITY + 1)

/** Legnth of the queue (10 items) */
#define QUEUE_LENGTH (10)

/** @brief Queue for buttons */
QueueHandle_t queue_buttons_1;
QueueHandle_t queue_buttons_2;

/** @brief Queue set */
QueueSetHandle_t queue_set;

/**
 * @brief Button 0 ISR
 *
 * Sends the delay time every time button 0 is pushed
 */
void GPIO_EVEN_IRQHandler(void) {
	uint32_t aux;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	uint32_t new_delay = pdMS_TO_TICKS(1000);

	/* clear flags */
	aux = GPIO_IntGet();
	GPIO_IntClear(aux);

	/* Send the data to the Queue */
	xQueueSendFromISR(queue_buttons_1, (void* ) &new_delay,
			&xHigherPriorityTaskWoken);

	/* Awake a task ? */
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * @brief Button 1 ISR
 *
 * Sends the delay time every time button 1 is pushed
 */
void GPIO_ODD_IRQHandler(void) {
	uint32_t aux;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	uint32_t new_delay = pdMS_TO_TICKS(250);

	/* clear flags */
	aux = GPIO_IntGet();
	GPIO_IntClear(aux);

	/* Send the data to the Queue */
	xQueueSendFromISR(queue_buttons_2, (void *) &new_delay,
			&xHigherPriorityTaskWoken);

	/* Awake a task ? */
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * @brief Our first task. A task is always a endless loop
 *
 * @param pParameter Unused parameter, present due to standard
 * 			task function prototype in FreeRTOS
 * @return none
 *
 */
static void TaskLedToggle(void *pParameter) {

	(void) pParameter;
	uint32_t my_delay;
	QueueSetMemberHandle_t selected_queue;

	/* Here optional init variables or functions */
	my_delay = pdMS_TO_TICKS(500);

	for (;;) {

		selected_queue = xQueueSelectFromSet(queue_set, 0);

		/* try to get new delay time from queue */
		if ( selected_queue == queue_buttons_1) {
			xQueueReceive(queue_buttons_1, &my_delay, (TickType_t ) 0);
		} else if (selected_queue == queue_buttons_2) {
			xQueueReceive(queue_buttons_2, &my_delay, (TickType_t ) 0);
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

	queue_set = xQueueCreateSet( QUEUE_LENGTH + QUEUE_LENGTH );

	/* Create Queues */
	queue_buttons_1 = xQueueCreate(QUEUE_LENGTH, sizeof(uint32_t));
	queue_buttons_2 = xQueueCreate(QUEUE_LENGTH, sizeof(uint32_t));

	xQueueAddToSet(queue_buttons_1, queue_set);
	xQueueAddToSet(queue_buttons_2, queue_set);

	/* Create our first task */
	xTaskCreate(TaskLedToggle, (const char *) "LedToggle",
			configMINIMAL_STACK_SIZE, NULL, TOGGLE_TASK_PRIORITY, NULL);

	/* Start FreeRTOS Scheduler */
	vTaskStartScheduler();

	return 0;
}

/**
 * @}
 */
