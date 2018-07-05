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

	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */
	GPIO_PinModeSet(gpioPortD, 8, gpioModeInput, 0); /* Boto 0 */
	GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 0); /* Boto 1 */

	/* Infinite loop */
	while (1) {
		if (GPIO_PinInGet(gpioPortD, 8) == 0) {
			GPIO_PinOutClear(gpioPortD, 7);
		}

		if (GPIO_PinInGet(gpioPortB, 11) == 0) {
			GPIO_PinOutSet(gpioPortD, 7);
		}

	}
}
