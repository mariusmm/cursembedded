/**
 ******************************************************************************
 * @file    GPIO_1.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-July-2017
 * @brief   Basic GPIO management
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"

int main(void) {
	/* Chip errata */
	CHIP_Init();

	CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_PinModeSet(gpioPortC, 0, gpioModePushPullDrive, 0); /* LED */
	GPIO_PinModeSet(gpioPortB, 9, gpioModeInput, 0); /* Boto 0 */
	GPIO_PinModeSet(gpioPortB, 10, gpioModeInput, 0); /* Boto 1 */

	/* Infinite loop */
	while (1) {
		if (GPIO_PinInGet(gpioPortB, 9) == 0) {
			GPIO_PinOutClear(gpioPortC, 0);
		}

		if (GPIO_PinInGet(gpioPortB, 10) == 0) {
			GPIO_PinOutSet(gpioPortC, 0);
		}

	}
}
