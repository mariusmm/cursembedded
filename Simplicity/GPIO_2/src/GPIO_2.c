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
	GPIO_PinOutClear(gpioPortD, 7);

}

void GPIO_ODD_IRQHandler(void) {
	uint32_t aux;

	aux = GPIO_IntGet();

	/* clear flags */
	GPIO_IntClear(aux);

	/* Set LED on */
	GPIO_PinOutSet(gpioPortD, 7);

}

int main(void) {
	/* Chip errata */
	CHIP_Init();

	CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */
	GPIO_PinModeSet(gpioPortD, 8, gpioModeInput, 0); /* Boto 0 */
	GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 0); /* Boto 1 */

	/* Set Interrupt configuration for both buttons */
	GPIO_IntConfig(gpioPortD, 8, false, true, true);
	GPIO_IntConfig(gpioPortB, 11, false, true, true);

	/* Enable interrupts */
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);

	/* Infinite loop */
	while (1) {
		/* nothings to do*/
		;
	}
}
