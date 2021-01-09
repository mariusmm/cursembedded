/*
 * Timer.cpp
 *
 *  Created on: 31 de des. 2020
 *      Author: marius
 */

#include "Timer.hpp"


volatile bool Timers::timer_triggered = false;

void LETIMER0_IRQHandler(void) {
	uint32_t flags;

	/* Clear flag for LETIMER0 */
	flags = LETIMER_IntGet(LETIMER0);
	LETIMER_IntClear(LETIMER0, flags);

	Timers::TimerISR();
}
