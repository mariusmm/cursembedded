/**
 ******************************************************************************
 * @file    PWM_2.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    09-March-2020
 * @brief   Using PWM to control a servo motor
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
 * PRESCALER = 8
 * TOP = ((14_000_000 / 50) / 8 )- 1 = 34999
 *
 * 0º -> 0,75 ms
 * 90º -> 1.5 ms
 * 180º -> 2.25 ms
 */

#define PWM_FREQ (34999)
//#define PWM_0 (1312)
#define PWM_0 (875)
#define PWM_180 (3937)

volatile uint32_t pwm_value = PWM_0;
volatile int32_t degree = 90;


uint32_t degrees_to_pwm(uint32_t degrees) {
	uint32_t ret_value = 0;

	if (degrees < 180) {
		ret_value = (degree * (PWM_180 - PWM_0) / 180 ) + PWM_0;
	} else {
		ret_value = PWM_180;
	}

	return ret_value;
}


void GPIO_EVEN_IRQHandler(void) {
	uint32_t aux;

	aux = GPIO_IntGet();

	/* clear flags */
	GPIO_IntClear(aux);

	degree += 30;
	if (degree > 180) degree = 180;
	pwm_value = degrees_to_pwm(degree);

	TIMER_CompareBufSet(TIMER1, 1, pwm_value);
}

void GPIO_ODD_IRQHandler(void) {
	uint32_t aux;

	aux = GPIO_IntGet();

	/* clear flags */
	GPIO_IntClear(aux);

	degree -= 30;
	if (degree < 0) {
		degree = 0;
	}
	pwm_value = degrees_to_pwm(degree);

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
		.prescale = timerPrescale8,
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

	/* Init value for servomotor */
	pwm_value = degrees_to_pwm(90);

	/* Set Timer */
	TIMER_InitCC(TIMER1, 1, &timerCCInit);
	TIMER1->ROUTE |= (TIMER_ROUTE_CC1PEN | TIMER_ROUTE_LOCATION_LOC4);

	TIMER_TopSet(TIMER1, PWM_FREQ);
	TIMER_CompareBufSet(TIMER1, 1, pwm_value );
	TIMER_Init(TIMER1, &timerInit);

	/* Enable interrupts */
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);

	/* Infinite loop */
	while (1) {
		/* nothings to do */
	}
}
