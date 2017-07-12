/**
 ******************************************************************************
 * @file    SpeedTest_2.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    12-July-2017
 * @brief   Demonstration of how fast is your microcontroller, 2nd version
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
 */

#include <stdio.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_timer.h"

#include "BSP.h"

#define TICKS_SECOND 13671

TIMER_Init_TypeDef timerInit =
  {
    .enable     = false,
    .debugRun   = false,
    .prescale   = timerPrescale1024,
    .clkSel     = timerClkSelHFPerClk,
    .fallAction = timerInputActionNone,
    .riseAction = timerInputActionNone,
    .mode       = timerModeUp,
    .dmaClrAct  = false,
    .quadModeX4 = false,
    .oneShot    = true,
    .sync       = false,
  };

int main(void) {

	uint32_t i;

	bool timer_running;
	uint32_t ticks;
	uint32_t total_time;

	/* Chip errata */
	CHIP_Init();

	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_TIMER0, true);

	setupSWOForPrint();

	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */
	GPIO_PinModeSet(gpioPortD, 8, gpioModeInput, 0); /* Boto 0 */
	GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 0); /* Boto 1 */

	/* configure TImer */
	TIMER_Init(TIMER0, &timerInit);

	i = 0;
	timer_running = false;

	/* Infinite loop */
	while (1) {

		/* while button 0 is pressed, CPU is counting */
		while (GPIO_PinInGet(gpioPortD, 8) == 0) {

			if (timer_running == false) {
				TIMER_CounterSet(TIMER0, 0);
				TIMER_Enable(TIMER0, true);
				timer_running = true;
			}

			i++;
		}

		TIMER_Enable(TIMER0, false);
		timer_running = false;

		ticks = TIMER_CounterGet(TIMER0);
		total_time = ticks * 1000 / TICKS_SECOND;

		if (i != 0) {
			printf("i = %lu\n", i);
			printf("Ticks = %lu\n", ticks);
			printf("time = %lu.%02lu\n", total_time / 1000, total_time % 1000);
			i = 0;
		}
	}
}
