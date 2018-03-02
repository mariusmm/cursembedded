/**
 ******************************************************************************
 * @file    UART_1.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    15-August-2017
 * @brief   Basic UART example
 * @license This project is released under the GNU Public License.
 *
 * Using USART1 #1 on PD0 (TX) & PD1 (RX) on Expansion header pins 4 & 6.
 * Serial port parameters ara: 115200 bps, 8 databits, no parity, 1 stop bit.
 *
 * When user push button 0 (1), a character is sent using USART1
 * (character 'A' or 'B').
 *
 * ******************************************************************************
 */

#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_usart.h"

USART_InitAsync_TypeDef usart_cfg = USART_INITASYNC_DEFAULT;

void GPIO_EVEN_IRQHandler(void) {
	uint32_t aux;

	aux = GPIO_IntGet();

	/* clear flags */
	GPIO_IntClear(aux);

	/* Set LED off */
	GPIO_PinOutClear(gpioPortD, 7);

	/* Send an A character */
	USART_Tx(USART1, 'A');

}

void GPIO_ODD_IRQHandler(void) {
	uint32_t aux;

	aux = GPIO_IntGet();

	/* clear flags */
	GPIO_IntClear(aux);

	/* Set LED on */
	GPIO_PinOutSet(gpioPortD, 7);

	/* Send an B character */
	USART_Tx(USART1, 'B');

}

int main(void) {
	/* Chip errata */
	CHIP_Init();

	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_USART1, true);

	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */

	GPIO_PinModeSet(gpioPortD, 8, gpioModeInput, 0); /* Boto 0 */
	GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 0); /* Boto 1 */

	GPIO_PinModeSet(gpioPortD, 0, gpioModePushPull, 1); /* PD0 USART1_TX */
	GPIO_PinModeSet(gpioPortD, 1, gpioModeInputPull, 1); /* PD1 USART1_RX */

	/* Set USART1 to 115200 bps */
	USART_InitAsync(USART1, &usart_cfg);

	/* Set route #1 for USART */
	USART1->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN
			| USART_ROUTE_LOCATION_LOC1;

	/* Send 'A' character */
	USART_Tx(USART1, 'A');

	/* Set Interrupt configuration for both buttons */
	GPIO_IntConfig(gpioPortD, 8, false, true, true);
	GPIO_IntConfig(gpioPortB, 11, false, true, true);

	/* Enable interrupts */
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);

	/* Infinite loop */
	while (1) {
		;
	}
}
