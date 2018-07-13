/**
 ******************************************************************************
 * @file    DAC_1.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    13-July-2018
 * @brief   Mode complex DAC example
 * @license This project is released under the GNU Public License
 * ******************************************************************************
 */

#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_dac.h"
#include "em_timer.h"

#include "BSP.h"

#define DAC_STEP 0x100
#define TOP_VALUE (875000 / 1000)

volatile int32_t DACvalue = 0x00;
volatile bool direction_up = true;

TIMER_Init_TypeDef timerInit = { .enable = false, .debugRun = false, .prescale =
		timerPrescale16, .clkSel = timerClkSelHFPerClk, .fallAction =
		timerInputActionNone, .riseAction = timerInputActionNone, .mode =
		timerModeUp, .dmaClrAct = false, .quadModeX4 = false, .oneShot = false,
		.sync = false, };

void TIMER0_IRQHandler(void) {
	uint32_t flags;

	/* Clear flag for TIMER0 */
	flags = TIMER_IntGet(TIMER0);
	TIMER_IntClear(TIMER0, flags);

	/* Toggle LED ON/OFF */
	GPIO_PinOutToggle(gpioPortD, 7);

	if (direction_up == true) {
		DACvalue += DAC_STEP;
		if (DACvalue >= 0x1000) {
			DACvalue = 0x0FFF;
			direction_up = false;
		}
	} else {
		DACvalue -= DAC_STEP;
		if (DACvalue < 0) {
			DACvalue = 0;
			direction_up = true;
		}
	}

	DAC_ChannelOutputSet(DAC0, 1, DACvalue);
}

static void TimerConfig(void) {
	CMU_ClockEnable(cmuClock_TIMER0, true);

	/* Enable overflow interrupt */
	TIMER_IntEnable(TIMER0, TIMER_IF_OF);

	/* Enable IRQ for Timer 0*/
	NVIC_EnableIRQ(TIMER0_IRQn);

	/* Set counter to 0  */
	TIMER_CounterSet(TIMER0, 0);

	/* Set TIMER Top value */
	TIMER_TopSet(TIMER0, TOP_VALUE);

	/* Configure Timer */
	TIMER_Init(TIMER0, &timerInit);

	/* Enable overflow interrupt */
	TIMER_IntEnable(TIMER0, TIMER_IF_OF);

	/* Enable IRQ for Timer 0*/
	NVIC_EnableIRQ(TIMER0_IRQn);

	/* Configure Timer */
	TIMER_Init(TIMER0, &timerInit);
}

/* (PB12 - DAC0_OUT1) */
static void DACConfig(void) {
	/* Use default settings */
	DAC_Init_TypeDef init = DAC_INIT_DEFAULT;
	DAC_InitChannel_TypeDef initChannel = DAC_INITCHANNEL_DEFAULT;

	/* Enable the DAC clock */
	CMU_ClockEnable(cmuClock_DAC0, true);

	init.prescale = DAC_PrescaleCalc(500000, 0);

	/* Set reference voltage to Vdd */
	init.reference = dacRefVDD;

	init.convMode = dacConvModeContinuous;

	init.refresh = dacRefresh8;
	initChannel.refreshEnable = true;

	/* Initialize the DAC and DAC channel #1 */
	DAC_Init(DAC0, &init);
	DAC_InitChannel(DAC0, &initChannel, 1);
}

int main(void) {

	/* Chip errata */
	CHIP_Init();

	setupSWOForPrint();

	CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */
	GPIO_PinModeSet(gpioPortD, 8, gpioModeInput, 0); /* Boto 0 */
	GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 0); /* Boto 1 */

	/* Set Interrupt configuration for both buttons */
	GPIO_IntConfig(gpioPortD, 8, false, true, true);
	GPIO_IntConfig(gpioPortB, 11, false, true, true);

	/* Configure DAC */
	DACConfig();

	/* Configure Timer */
	TimerConfig();

	/* Start DAC */
	DAC_Enable(DAC0, 1, true);

	/* Start Timer */
	TIMER_Enable(TIMER0, true);

	/* Infinite loop */
	while (1) {
		/* nothing to do here */
	}
}

/**
 * @mainpage DAC Application example
 * This example outputs a variable value using DAC0 channel 1 (PB12 - DAC0_OUT1)
 * Timer 0 is triggering an IRQ every 1 ms.
 * The Timer ISR writes a new value to the DAC
 * @}
 */

