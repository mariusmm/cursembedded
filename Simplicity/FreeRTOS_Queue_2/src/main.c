/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    28-May-2018
 * @brief   More advanced FreeRTOS queue example
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

/** Structure to to send & received by tbe queue */
struct queue_pkt {
	uint32_t data_A;	/**< arbitrary field, we'll use it to store delay */
	uint8_t data_B;		/**< arbitrary field */
	bool data_c;		/**< arbitrary field */
};

/**
 * @brief Button 0 ISR
 *
 * Sends the delay time every time button 0 is pushed
 */
void GPIO_EVEN_IRQHandler(void) {
	uint32_t aux;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	struct queue_pkt pkt;
	pkt.data_A = pdMS_TO_TICKS(1000);

	/* clear flags */
	aux = GPIO_IntGet();
	GPIO_IntClear(aux);

	/* Send the data to the Queue */
	xQueueSendFromISR(queue_buttons, (void* ) &pkt,
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

	struct queue_pkt pkt;
	pkt.data_A = pdMS_TO_TICKS(250);

	/* clear flags */
	aux = GPIO_IntGet();
	GPIO_IntClear(aux);

	/* Send the data to the Queue */
	xQueueSendFromISR(queue_buttons, (void* ) &pkt,
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

	struct queue_pkt pkt;

	/* Here optional init variables or functions */

	my_delay = pdMS_TO_TICKS(500);

	for (;;) {

		/* try to get new delay time from queue */
		if (xQueueReceive(queue_buttons, &pkt, (TickType_t ) 0)) {
			my_delay = pkt.data_A;
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
	queue_buttons = xQueueCreate(QUEUE_LENGTH, sizeof(struct queue_pkt));

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

/**
 * @mainpage FreeRTOS advanced queue example
 *
 *
 * This examples uses a queue (@ref queue_buttons) to send data from two ISRs to a task.
 * In the example a struct (struct queue_pkt) is sent by the queue, but only a field is being used.
 *
 * The ISRs are attached to button 0 & 1 IRQs (@ref GPIO_EVEN_IRQHandler, @ref GPIO_ODD_IRQHandler)
 * and each one sends a different value
 * through the queue
 *
 * The task (@ref TaskLedToggle) tries to read from the queue, and if it is data, it waits for the received
 * amount of time (delay) to toggle the LED. This data is in the field data_A of the struct queue_pkt.
 *
 *
 * This project uses the following libraries:
 * @li @ref BSP
 *
 * @}
 */
