/*
 * SystemDelay.c
 *
 *  Created on: 19 de des. 2020
 *      Author: marius
 */

#include <stdint.h>

#include "em_device.h"
#include "em_cmu.h"

#include "definitions.h"
#include "SystemDelay.h"

volatile uint32_t msTicks;

void SysTick_Handler(void) {
	msTicks++;
}

static uint32_t getSysTicks(void) {
	return msTicks;
}

void SysTicksInit(void) {
	SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / SYSTICKS_PER_SECOND);
}

void SysTicksDelay(uint32_t delay) {
	uint32_t start = getSysTicks();
	uint32_t now;

	do {
		now = getSysTicks();
	} while (now - start < delay);
}

void SysTicksDisable(void) {
	SysTick->CTRL = 0;
}

void SysTicksEnable(void) {
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
	SysTick_CTRL_TICKINT_Msk |
	SysTick_CTRL_ENABLE_Msk;
}
