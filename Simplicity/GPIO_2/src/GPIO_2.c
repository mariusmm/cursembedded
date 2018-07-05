/**
 ******************************************************************************
 * @file    GPIO_2.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-July-2017
 * @brief   GPIO with IRQ example
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"

void GPIO_EVEN_IRQHandler(void) {
	uint32_t aux;

	aux = GPIO_IntGet();

	/* clear flags */
	GPIO_IntClear(aux);

	/* Set LED off */
	GPIO_PinOutClear(gpioPortC, 0);
}

void GPIO_ODD_IRQHandler(void) {
	uint32_t aux;

	aux = GPIO_IntGet();

	/* clear flags */
	GPIO_IntClear(aux);

	/* Set LED on */
	GPIO_PinOutSet(gpioPortC, 0);
}

int main(void) {
	/* Chip errata */
	CHIP_Init();

	CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_PinModeSet(gpioPortC,  0, gpioModePushPullDrive, 0); /* LED */
	GPIO_PinModeSet(gpioPortB,  9, gpioModeInput, 0); /* Boto 0 */
	GPIO_PinModeSet(gpioPortB, 10, gpioModeInput, 0); /* Boto 1 */

	/* Set Interrupt configuration for both buttons */
	GPIO_IntConfig(gpioPortB,  9, false, true, true);
	GPIO_IntConfig(gpioPortB, 10, false, true, true);

	/* Enable interrupts */
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);

	/* Infinite loop */
	while (1) {
		/* nothings to do*/
		;
	}
}
