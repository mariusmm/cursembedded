/**
 ******************************************************************************
 * @file    Datatypes.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    21-July-2017
 * @brief   Datatypes examples with some performance test
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
#include "Tests.h"

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

	uint32_t ticks;
	uint32_t total_time;

	/* Chip errata */
	CHIP_Init();

	setupSWOForPrint();

	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_TIMER0, true);

	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */
	GPIO_PinModeSet(gpioPortD, 8, gpioModeInput, 0); /* Boto 0 */
	GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 0); /* Boto 1 */

	printf("************\r\n");
	printf("Size of char %d\r\n", sizeof(char));
	printf("Size of short int %d\r\n", sizeof(short int));
	printf("Size of int %d\r\n", sizeof(int));
	printf("Size of long int %d\r\n", sizeof(long int));
	printf("Size of float %d\r\n", sizeof(float));
	printf("Size of double %d\r\n", sizeof(double));
	printf("************\r\n");
	printf("Size of uint8_t %d\r\n", sizeof(uint8_t));
	printf("Size of uint16_t %d\r\n", sizeof(uint16_t));
	printf("Size of uint32_t %d\r\n", sizeof(uint32_t));
	printf("************\r\n");
	printf("Size of int8_t %d\r\n", sizeof(int8_t));
	printf("Size of int16_t %d\r\n", sizeof(int16_t));
	printf("Size of int32_t %d\r\n", sizeof(int32_t));
	printf("************\r\n");
	printf("Size of *uint8_t %d\r\n", sizeof(uint8_t *));
	printf("Size of *uint16_t %d\r\n", sizeof(uint16_t *));
	printf("Size of *uint32_t %d\r\n", sizeof(uint32_t *));
	printf("Size of *char %d\r\n", sizeof(char *));
	printf("Size of *void %d\r\n", sizeof(void *));
	printf("************\r\n");

	/* configure Timer to measure time for the tests */
	TIMER_Init(TIMER0, &timerInit);

	/* First Test */
	printf("****** 1st test ******\r\n");
	TIMER_CounterSet(TIMER0, 0);
	TIMER_Enable(TIMER0, true);
	Test1();
	TIMER_Enable(TIMER0, false);
	ticks = TIMER_CounterGet(TIMER0);
	total_time = ticks * 1000 / TICKS_SECOND;
	printf("Ticks = %lu\n", ticks);
	printf("time = %lu.%02lu\n", total_time / 1000, total_time % 1000);

	/* Second test */
	printf("****** 2nd test ******\r\n");
	TIMER_CounterSet(TIMER0, 0);
	TIMER_Enable(TIMER0, true);
	Test2();
	TIMER_Enable(TIMER0, false);
	ticks = TIMER_CounterGet(TIMER0);
	total_time = ticks * 1000 / TICKS_SECOND;
	printf("Ticks = %lu\n", ticks);
	printf("time = %lu.%02lu\n", total_time / 1000, total_time % 1000);

	/* Third test */
	printf("****** 3rd test ******\r\n");
	TIMER_CounterSet(TIMER0, 0);
	TIMER_Enable(TIMER0, true);
	Test3();
	TIMER_Enable(TIMER0, false);
	ticks = TIMER_CounterGet(TIMER0);
	total_time = ticks * 1000 / TICKS_SECOND;
	printf("Ticks = %lu\n", ticks);
	printf("time = %lu.%02lu\n", total_time / 1000, total_time % 1000);

	/* Fourth test */
	printf("****** 4th test ******\r\n");
	TIMER_CounterSet(TIMER0, 0);
	TIMER_Enable(TIMER0, true);
	Test4();
	TIMER_Enable(TIMER0, false);
	ticks = TIMER_CounterGet(TIMER0);
	total_time = ticks * 1000 / TICKS_SECOND;
	printf("Ticks = %lu\n", ticks);
	printf("time = %lu.%02lu\n", total_time / 1000, total_time % 1000);

	/* Fifth test */
	printf("****** 5th test ******\r\n");
	TIMER_CounterSet(TIMER0, 0);
	TIMER_Enable(TIMER0, true);
	Test5();
	TIMER_Enable(TIMER0, false);
	ticks = TIMER_CounterGet(TIMER0);
	total_time = ticks * 1000 / TICKS_SECOND;
	printf("Ticks = %lu\n", ticks);
	printf("time = %lu.%02lu\n", total_time / 1000, total_time % 1000);


	/* Infinite loop */
	while (1) {
		/* nothings to do*/
		;
	}
}
