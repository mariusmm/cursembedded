/*
 * timers.c
 *
 *  Created on: 20 de des. 2020
 *      Author: marius
 */


#include "em_cmu.h"
#include "em_letimer.h"

#include "definitions.h"
#include "RegisterFile.h"

volatile bool timer_triggered;

void Timers_init(void) {

	CMU_ClockEnable(cmuClock_HFLE, true);
	CMU_ClockEnable(cmuClock_LETIMER0, true);
	CMU_ClockDivSet(cmuClock_LETIMER0, cmuClkDiv_2048);

	LETIMER_Init_TypeDef letimerInit = LETIMER_INIT_DEFAULT;

	LETIMER_IntEnable(LETIMER0, TIMER_IF_UF);
	LETIMER_TopSet(LETIMER0, 6500);
	LETIMER_Init(LETIMER0, &letimerInit);

	NVIC_EnableIRQ(LETIMER0_IRQn);

	timer_triggered = false;
}

void Timers_setDuty(uint16_t duty) {

	// 1 cycles / sec
	uint16_t calc = CMU_ClockFreqGet(cmuClock_LETIMER0);
	calc = calc * duty;
	LETIMER_TopSet(LETIMER0, calc);
	LETIMER_CounterSet(LETIMER0, calc);
	LETIMER_Enable(LETIMER0, true);
}

bool Timers_triggered() {
	return timer_triggered;
}

void Timers_processed() {
	timer_triggered = false;
}

void DutyUpdate(int unused) {
	uint16_t new_duty = 0;
	new_duty = register_direct_read(REGISTER_DUTYCYLE);
	Timers_setDuty(new_duty);
}

void LETIMER0_IRQHandler(void) {
	uint32_t flags;

	/* Clear flag for LETIMER0 */
	flags = LETIMER_IntGet(LETIMER0);
	LETIMER_IntClear(LETIMER0, flags);

	timer_triggered = true;
}

