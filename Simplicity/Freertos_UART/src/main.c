/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    8-March-2018
 * @brief   USART & FreeRTOS example
 * @license This project is released under the GNU Public License.s
 *
 * Same example as USART_2 using FreeRTOS and queues instead of
 * a circular buffer.
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
#include "em_usart.h"

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
QueueHandle_t USART_RX_queue;
QueueHandle_t USART_TX_queue;

void USART_Send(USART_TypeDef *usart);

void USART1_TX_IRQHandler(void) {
	char data;

	USART_IntClear( USART1, USART_IEN_TXC);
	if (xQueueReceiveFromISR(USART_TX_queue, &data, 0) == pdTRUE) {
			USART_Tx(USART1, data);
		}
}

void USART1_RX_IRQHandler(void) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	char data;

	if (USART1->IF & LEUART_IF_RXDATAV) {
		data = USART_Rx(USART1);
		xQueueSendFromISR(USART_RX_queue, (void*) &data, &xHigherPriorityTaskWoken);
		USART_IntClear( USART1, USART_IEN_RXDATAV);
	}

	/* Awake a task ? */
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void USART_Send(USART_TypeDef *usart) {
	int data;

	if (xQueueReceive(USART_TX_queue, &data, 0) == pdTRUE) {
		USART_Tx(USART1, data);
	}
}

/**
 * @brief Our first task. A task is always a endless loop
 *
 * @param pParameter Unused parameter
 * @return none
 *
 */
static void UARTTAsk(void *pParameter) {

	(void) pParameter;
	char recv_char;
	char tx_char;

	for (;;) {
		/* try to get new delay time from queue */
		if (xQueueReceive(USART_RX_queue, &recv_char, portMAX_DELAY)) {
			tx_char = recv_char;
			xQueueSend(USART_TX_queue, &tx_char, 0);
			tx_char++;
			xQueueSend(USART_TX_queue, &tx_char, 0);
			tx_char++;
			xQueueSend(USART_TX_queue, &tx_char, 0);
			USART_Send(USART1);
		}
	}
}


void USART_Init(USART_TypeDef *usart) {

	CMU_ClockEnable(cmuClock_USART1, true);

	USART_InitAsync_TypeDef init = {
			usartEnable, /* Enable RX/TX when init completed. */
			0, /* Use current configured reference clock for configuring baudrate. */
			115200, /* 115200 bits/s. */
			usartOVS16, /* 16x oversampling. */
			usartDatabits8, /* 8 databits. */
			usartNoParity, /* No parity. */
			usartStopbits1, /* 1 stopbit. */
			false, /* Do not disable majority vote. */
			false, /* Not USART PRS input mode. */
			usartPrsRxCh0 /* PRS channel 0. */
	};

	/* Define output (TX) */
	GPIO_PinModeSet(gpioPortD, 0, gpioModePushPull, 1);

	/* Define input, no filtering */
	GPIO_PinModeSet(gpioPortD, 1, gpioModeInput, 0);

	/* Configure USART for basic async operation */
	USART_InitAsync(usart, &init);

	/* Clear & activate RX interrupt */
	USART_IntClear(USART1, USART_IEN_RXDATAV);
	USART_IntEnable(USART1, USART_IEN_RXDATAV);

	/* Clear & activate TX interrupt */
	USART_IntClear(USART1, USART_IEN_TXC);
	USART_IntEnable(USART1, USART_IEN_TXC);

	NVIC_ClearPendingIRQ(USART1_RX_IRQn);
	NVIC_ClearPendingIRQ(USART1_TX_IRQn);

	NVIC_SetPriority(USART1_RX_IRQn, 5);
	NVIC_SetPriority(USART1_TX_IRQn, 5);

	NVIC_EnableIRQ(USART1_RX_IRQn);
	NVIC_EnableIRQ(USART1_TX_IRQn);

	/* Set route #1 for USART */
	usart->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN
			| USART_ROUTE_LOCATION_LOC1;

	/* Finally enable it */
	USART_Enable(usart, usartEnable);
}
/**
 * @brief main function
 */
int main(void) {
	/* Chip errata */
	CHIP_Init();

	/* Init functions for our BSP */
	BSP_Init();

	/* Init USART1 */
	USART_Init(USART1);

	/* Init SLEEP library */
	SLEEP_Init(NULL, NULL);
#if (configSLEEP_MODE < 3)
	SLEEP_SleepBlockBegin((SLEEP_EnergyMode_t) (configSLEEP_MODE));
#endif

	/* Create Queue */
	USART_TX_queue = xQueueCreate(64, sizeof(char));
	USART_RX_queue = xQueueCreate(64, sizeof(char));

	if ( (USART_RX_queue == NULL) || (USART_TX_queue == NULL) ) {
		while(1){

		}
	}
	/* Create our first task */
	xTaskCreate(UARTTAsk, (const char *) "UARTTAsk",
	configMINIMAL_STACK_SIZE, NULL, TOGGLE_TASK_PRIORITY, NULL);

	/* Send 'A' character */
	USART_Tx(USART1, 'A');

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
