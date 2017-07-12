/**
 ******************************************************************************
 * @file    SpeedTest_1.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    12-July-2017
 * @brief   Demonstration of how fast is your microcontroller
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
 */

#include <stdio.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"

#include "BSP.h"

uint32_t i;

int main(void) {
	/* Chip errata */
	CHIP_Init();

	CMU_ClockEnable(cmuClock_GPIO, true);

	setupSWOForPrint();

	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */
	GPIO_PinModeSet(gpioPortD, 8, gpioModeInput, 0); 		 /* Boto 0 */
	GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 0); 		 /* Boto 1 */

	i = 0;

	/* Infinite loop */
	while (1) {

		/* while button 0 is pressed, CPU is counting */
		while (GPIO_PinInGet(gpioPortD, 8) == 0) {
			i++;
		}

		if (i != 0) {
			printf("i = %lu\n", i);
			i = 0;
		}
	}
}
