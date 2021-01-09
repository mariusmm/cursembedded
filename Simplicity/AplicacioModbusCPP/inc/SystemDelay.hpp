/*
 * SystemDelay.hpp
 *
 *  Created on: 21 de des. 2020
 *      Author: marius
 */

#ifndef INC_SYSTEMDELAY_HPP_
#define INC_SYSTEMDELAY_HPP_

#include <definitions.hpp>
#include <stdint.h>

#include "em_device.h"
#include "em_cmu.h"


class SysTick_class {
public:
	SysTick_class() {
		SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / SYSTICKS_PER_SECOND);
		msTicks = 0;
		duty_cycle = 0;
	}

	void inc(void) {
		msTicks++;
	}

	uint32_t get(void) {
		return msTicks;
	}

	void Disable(void) {
		SysTick->CTRL = 0;
	}

	void Enable(void) {
		SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
		SysTick_CTRL_TICKINT_Msk |
		SysTick_CTRL_ENABLE_Msk;
	}

	void Delay(uint32_t delay) {
		uint32_t start = this->get();
		uint32_t now;

		do {
			now = this->get();
		} while (now - start < delay);
	}

private:
	/* volatile to protect it from compiler optimizations */
	volatile uint32_t msTicks;
	uint32_t duty_cycle;
};

SysTick_class SysTick_mng;

void SysTick_Handler(void) {
	SysTick_mng.inc();
}

#endif /* INC_SYSTEMDELAY_HPP_ */
