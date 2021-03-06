/**
 ******************************************************************************
 * @file    PWM_1.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-July-2017
 * @brief   Using PWM example
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
 */

#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_timer.h"

/**
 * PD7 (Led) is connected to TIM1_CC1 #4
 *
 * PWM Frequency is : 14_000_000 / 256 / 4000 = 13,67 Hz
 *
 */

#define PWM_FREQ 4000

volatile uint32_t pwm_value = PWM_FREQ/6;

void GPIO_EVEN_IRQHandler(void) {
	uint32_t aux;

	aux = GPIO_IntGet();

	/* clear flags */
	GPIO_IntClear(aux);

	/* Decrease PWM duty */
	pwm_value -= PWM_FREQ / 6;
	TIMER_CompareBufSet(TIMER1, 1, pwm_value);

}

void GPIO_ODD_IRQHandler(void) {
	uint32_t aux;

	aux = GPIO_IntGet();

	/* clear flags */
	GPIO_IntClear(aux);

	/* Increase PWM duty */
	pwm_value += PWM_FREQ / 6;
	TIMER_CompareBufSet(TIMER1, 1, pwm_value);
}

TIMER_InitCC_TypeDef timerCCInit = {
		.eventCtrl = timerEventEveryEdge,
		.edge =	timerEdgeNone,
		.prsSel = timerPRSSELCh0,
		.cufoa = timerOutputActionNone,
		.cofoa = timerOutputActionSet,
		.cmoa = timerOutputActionClear,
		.mode =	timerCCModePWM,
		.filter = false,
		.prsInput = false,
		.coist = false,
		.outInvert = false,
	};

TIMER_Init_TypeDef timerInit = {
		.enable = true,
		.debugRun = true,
		.prescale = timerPrescale256,
		.clkSel = timerClkSelHFPerClk,
		.fallAction = timerInputActionNone,
		.riseAction = timerInputActionNone,
		.mode =	timerModeUp,
		.dmaClrAct = false,
		.quadModeX4 = false,
		.oneShot = false,
		.sync = false,
	};

int main(void) {
	/* Chip errata */
	CHIP_Init();

	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_TIMER1, true);

	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */
	GPIO_PinModeSet(gpioPortD, 8, gpioModeInput, 0); /* Boto 0 */
	GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 0); /* Boto 1 */

	/* Set Interrupt configuration for both buttons */
	GPIO_IntConfig(gpioPortD, 8, false, true, true);
	GPIO_IntConfig(gpioPortB, 11, false, true, true);

	/* Set Timer */
	TIMER_InitCC(TIMER1, 1, &timerCCInit);
	TIMER1->ROUTE |= (TIMER_ROUTE_CC1PEN | TIMER_ROUTE_LOCATION_LOC4);

	TIMER_TopSet(TIMER1, PWM_FREQ);
	TIMER_CompareBufSet(TIMER1, 1, pwm_value);
	TIMER_Init(TIMER1, &timerInit);

	/* Enable interrupts */
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);

	/* Infinite loop */
	while (1) {
		/* nothings to do */
		;
	}
}
