/**
 ******************************************************************************
 * @file    UART_2.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    2-March-2018
 * @brief   UART example
 * @license This project is released under the GNU Public License.
 *
 * Using USART1 #1 on PD0 (TX) & PD1 (RX) on Expansion header pins 4 & 6.
 * Serial port parameters are: 115200 bps, 8 databits, no parity, 1 stop bit.
 *
 * This example receives a character and outputs the same character and the
 * 2 following characters in the ASCII table.
 * All USART related functions are using IRQs instead of polling
 *
 * ******************************************************************************
 */

#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_usart.h"

#include "CircularBuffer.h"

USART_InitAsync_TypeDef usart_cfg = USART_INITASYNC_DEFAULT;

static CircularBuffer_t TX_SerialBuffer,
RX_SerialBuffer;

void USART1_TX_IRQHandler(void) {
	USART_IntClear( USART1, USART_IEN_TXC);
	USART_Send(USART1);
}

void USART1_RX_IRQHandler(void) {
	char data;

	if (USART1->IF & LEUART_IF_RXDATAV) {
		data = USART_Rx(USART1);
		PushData(data, &RX_SerialBuffer);
		USART_IntClear( USART1, USART_IEN_RXDATAV);
	}
}

void USART_Send(USART_TypeDef *usart) {
	int data;

	if (AvailableData(&TX_SerialBuffer) != 0) {
		data = PopData(&TX_SerialBuffer);
		USART_Tx(USART1, data);
	}
}

void USART_Init(USART_TypeDef *usart) {

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

	NVIC_EnableIRQ(USART1_RX_IRQn);
	NVIC_EnableIRQ(USART1_TX_IRQn);

	/* Set route #1 for USART */
	usart->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN
			| USART_ROUTE_LOCATION_LOC1;

	/* Finally enable it */
	USART_Enable(usart, usartEnable);
}

int main(void) {
	/* Chip errata */
	CHIP_Init();
	int character;

	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_USART1, true);

	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */

	USART_Init(USART1);

	/* Send 'A' character */
	USART_Tx(USART1, 'A');

	/* Infinite loop */
	while (1) {
		if (AvailableData(&RX_SerialBuffer) != 0) {
			character = PopData(&RX_SerialBuffer);

			/* Prepare the buffer with the data to be sent */
			PushData(character, &TX_SerialBuffer);
			PushData(character + 1, &TX_SerialBuffer);
			PushData(character + 2, &TX_SerialBuffer);
			/* Start send process */
			USART_Send(USART1);
		}
	}
}

