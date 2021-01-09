/*
 * Timer.hpp
 *
 *  Created on: 30 de des. 2020
 *      Author: marius
 */

#ifndef INC_TIMER_HPP_
#define INC_TIMER_HPP_

#include <cstdint>
#include <atomic>

#include "em_cmu.h"
#include "em_letimer.h"
#include "em_timer.h"

class Timers {

public:
	Timers() {
		CMU_ClockEnable(cmuClock_HFLE, true);
		CMU_ClockEnable(cmuClock_LETIMER0, true);
		CMU_ClockDivSet(cmuClock_LETIMER0, cmuClkDiv_2048);

		LETIMER_Init_TypeDef letimerInit = LETIMER_INIT_DEFAULT;

		LETIMER_IntEnable(LETIMER0, TIMER_IF_UF);
		LETIMER_TopSet(LETIMER0, 6500);
		LETIMER_Init(LETIMER0, &letimerInit);

		NVIC_EnableIRQ(LETIMER0_IRQn);
	}

	static void SetDuty(uint16_t duty) {
		// 1 cycles / sec
		uint16_t calc = CMU_ClockFreqGet(cmuClock_LETIMER0);
		calc = calc * duty;
		LETIMER_TopSet(LETIMER0, calc);
		LETIMER_CounterSet(LETIMER0, calc);
		LETIMER_Enable(LETIMER0, true);
	}

	bool Triggered() {
		return timer_triggered;
	}

	void Processed() {
		timer_triggered = false;
	}

	void DutyUpdate(int new_duty) {
		this->SetDuty(new_duty);
	}

	static void TimerISR() {
		timer_triggered = true;
	}

public:
	static volatile bool timer_triggered;
};


#endif /* INC_TIMER_HPP_ */
