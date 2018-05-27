/*
 * PWM.c
 *
 *  Created on: 23 maig 2018
 *      Author: marius
 */

#include "PWM.h"

void PWM_Init(void) {
	TIMER_InitCC_TypeDef timerCCInit =
			{ .eventCtrl = timerEventEveryEdge, .edge = timerEdgeNone, .prsSel =
					timerPRSSELCh0, .cufoa = timerOutputActionNone, .cofoa =
					timerOutputActionSet, .cmoa = timerOutputActionClear,
					.mode = timerCCModePWM, .filter = false, .prsInput = false,
					.coist = false, .outInvert = false, };

	TIMER_Init_TypeDef timerInit =
			{ .enable = true, .debugRun = true, .prescale = timerPrescale256,
					.clkSel = timerClkSelHFPerClk, .fallAction =
							timerInputActionNone, .riseAction =
							timerInputActionNone, .mode = timerModeUp,
					.dmaClrAct = false, .quadModeX4 = false, .oneShot = false,
					.sync = false, };

	CMU_ClockEnable(cmuClock_TIMER1, true);

	/* Set Timer */
	TIMER_InitCC(TIMER1, 1, &timerCCInit);
	TIMER1->ROUTE |= (TIMER_ROUTE_CC1PEN | TIMER_ROUTE_LOCATION_LOC4);

	TIMER_TopSet(TIMER1, PWM_FREQ);
	TIMER_CompareBufSet(TIMER1, 1, 1000);
	TIMER_Init(TIMER1, &timerInit);
}


void PWM_Set(uint8_t percentage) {
	uint32_t pwm_value;

	if (percentage > 100) {
		percentage = 100;
	}

	/* convert to percentage (0 to 100) to range 0 - PWM_FREQ */
	pwm_value = percentage * 4096 / 100;

	TIMER_CompareBufSet(TIMER1, 1, pwm_value);
}
