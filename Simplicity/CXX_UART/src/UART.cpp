/**
 ******************************************************************************
 * @file    UART.cpp
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    06-Aug-2018
 * @brief	C++ based driver for UART
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
 */

#include <UART.h>

UART::UART(unsigned long baudrate, unsigned int uart_number,
		unsigned int location, bool use_irq) {

	USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;

	init.baudrate = baudrate;

	CMU_ClockEnable(cmuClock_GPIO, true);

	if (uart_number == 0) {
		m_uart = USART1;
		CMU_ClockEnable(cmuClock_USART0, true);

	} else if (uart_number == 1) {

		m_uart = USART1;
		CMU_ClockEnable(cmuClock_USART1, true);
	} else {
		// What to do if is an error here?
	}

	/* Configure USART for basic async operation */
	USART_InitAsync(m_uart, &init);

	switch (location) {
	case 0: {
		/* Set route #0 for USART */
		m_uart->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN
				| USART_ROUTE_LOCATION_LOC0;
		break;
	}
	case 1: {
		/* Set route #1 for USART */

		/* Define output (TX) */
		GPIO_PinModeSet(gpioPortD, 0, gpioModePushPull, 1);

		/* Define input, no filtering */
		GPIO_PinModeSet(gpioPortD, 1, gpioModeInput, 0);

		m_uart->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN
				| USART_ROUTE_LOCATION_LOC1;
		break;
	}
	case 2: {
		/* Set route #2 for USART */
		m_uart->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN
				| USART_ROUTE_LOCATION_LOC2;
		break;
	}
	case 3: {
		/* Set route #3 for USART */
		m_uart->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN
				| USART_ROUTE_LOCATION_LOC3;
		break;
	}
	case 4: {
		/* Set route #4 for USART */
		m_uart->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN
				| USART_ROUTE_LOCATION_LOC4;
		break;
	}
	case 5: {
		/* Set route #5 for USART */
		m_uart->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN
				| USART_ROUTE_LOCATION_LOC5;
		break;
	}
	default: {
		// what to do?
	}
	}

	if (use_irq == true) {
		/* Clear & activate RX interrupt */
		USART_IntClear(m_uart, USART_IEN_RXDATAV);
		USART_IntEnable(m_uart, USART_IEN_RXDATAV);

		/* Clear & activate TX interrupt */
		USART_IntClear(m_uart, USART_IEN_TXC);
		USART_IntEnable(m_uart, USART_IEN_TXC);

		if (uart_number == 0) {
			NVIC_ClearPendingIRQ(USART0_RX_IRQn);
			NVIC_ClearPendingIRQ(USART0_TX_IRQn);

			NVIC_EnableIRQ(USART0_RX_IRQn);
			NVIC_EnableIRQ(USART0_TX_IRQn);
		} else if (uart_number == 1) {
			NVIC_ClearPendingIRQ(USART1_RX_IRQn);
			NVIC_ClearPendingIRQ(USART1_TX_IRQn);

			NVIC_EnableIRQ(USART1_RX_IRQn);
			NVIC_EnableIRQ(USART1_TX_IRQn);
		}
	}

	USART_Enable(m_uart, usartEnable);
}

UART::~UART() {
	USART_Enable(m_uart, usartDisable);
}

void UART::Tx(unsigned char c) const {
	USART_Tx(m_uart, c);
}

void UART::Send() {
	int data;

	if (m_TX.AvailableData() != 0) {
		data = m_TX.PopData();
		Tx(data);
	}
}

int UART::AvailableData() {
	return m_RX.AvailableData();
}

int UART::GetData() {
	return m_RX.PopData();
}

void UART::SendData(char c) {
	m_TX.PushData(c);
}

void UART::USART1_TX_IRQHandler(void) {
	USART_IntClear( USART1, USART_IEN_TXC);
	Send();
}

void UART::USART1_RX_IRQHandler(void) {
	char data;

	if (USART1->IF & LEUART_IF_RXDATAV) {
		data = USART_Rx(USART1);
		m_RX.PushData(data);
		USART_IntClear( USART1, USART_IEN_RXDATAV);
	}
}

