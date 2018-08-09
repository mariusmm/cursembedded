/**
 ******************************************************************************
 * @file    CXX_UART.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    06-Aug-2018
 * @brief	C++ based project using UART, the funcionality is the same as UART_2
 * example
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
 */

#include "em_device.h"
#include "em_chip.h"

#include <ostream>
#include "UART.h"

UART* helper_uart;

void USART1_TX_IRQHandler() {
	helper_uart->USART1_TX_IRQHandler();
}

void USART1_RX_IRQHandler() {
	helper_uart->USART1_RX_IRQHandler();
}

int main(void) {
	/* Chip errata */
	CHIP_Init();

	UART my_uart(115200, 1, 1, true);
	helper_uart = &my_uart;

#if defined(USE_CHAR_OP) || defined(USE_STRING_OP)
	my_uart << "Testing" << " C++ string style";
#else
	char str[] = "Testing C++ string style";
	for(char* it = str; *it; ++it) {
		my_uart.Tx(*it);
	}
#endif

	int character;

	/* Infinite loop */
	while (1) {

		if (my_uart.AvailableData() != 0) {
			character = my_uart.GetData();

			/* Prepare the buffer with the data to be sent */
			my_uart.SendData(character);
			my_uart.SendData(character + 1);
			my_uart.SendData(character + 2);
			/* Start send process */
			my_uart.Send();
		}
	}
}
