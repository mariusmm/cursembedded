/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    25-July-2017
 * @brief   Basic FreeRTOS Queue example
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
QueueHandle_t queue_buttons;

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
	xQueueSendFromISR(queue_buttons, (void* ) &new_delay,
			&xHigherPriorityTaskWoken);

	/* Awake a task ? */
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * @brief Button 1 ISR
 *
 * Sends the delay time every time button 0 is pushed
 */
void GPIO_ODD_IRQHandler(void) {
	uint32_t aux;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	uint32_t new_delay = pdMS_TO_TICKS(250);

	/* clear flags */
	aux = GPIO_IntGet();
	GPIO_IntClear(aux);

	/* Send the data to the Queue */
	xQueueSendFromISR(queue_buttons, (void* ) &new_delay,
			&xHigherPriorityTaskWoken);

	/* Awake a task ? */
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
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
	uint32_t my_delay;
	uint32_t recv_delay;

	/* Here optional init variables or functions */

	my_delay = pdMS_TO_TICKS(500);

	for (;;) {

		/* try to get new delay time from queue */
		if (xQueueReceive(queue_buttons, &recv_delay, (TickType_t ) 0)) {
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
	queue_buttons = xQueueCreate(QUEUE_LENGTH, sizeof(uint32_t));

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
 * @mainpage FreeRTOS basic queue example
 *
 *
 * This examples uses a queue to send data from two ISRs to a task
 *
 * The ISRs are attached to button 0 & 1IRQs and each one sends a different value
 * through the queue
 *
 * The task tries to read from the queue, and if it is data, it waits for the received
 * amount of time (delay) to toggle the LED
 *
 *
 * This project uses the following libraries:
 * @li @ref BSP
 *
 * @}
 */
