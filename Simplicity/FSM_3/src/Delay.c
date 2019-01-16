/**
 ******************************************************************************
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @file    Delay.c
 * @version V1.0
 * @date    15-January-2019
 * @brief   Delay functions
 * @license This project is released under the GNU Public License
 *
 * Taken from https://www.silabs.com/community/mcu/32-bit/forum.topic.html/rtc_for_generic_dela-UtHh
 * ******************************************************************************
 */

#include "Delay.h"

volatile bool timer_finished;

RTCDRV_TimerID_t delay_timer;

/**
 * Timer callback
 * @param id Timer id
 * @param user extra parameter (unused)
 */
static void Timer_Expired(RTCDRV_TimerID_t id, void* user) {
	timer_finished = true; // breakpoint here is always hit
}

void Delay_Init() {
	RTCDRV_Init();
	RTCDRV_AllocateTimer(&delay_timer);
}

void Delay(int ms) {
	timer_finished = false;
	RTCDRV_StartTimer(delay_timer, rtcdrvTimerTypeOneshot, ms, Timer_Expired,
	NULL);
	while (!timer_finished) {
		;
	}
}

void EM1_Delay(int ms) {
	timer_finished = false;
	RTCDRV_StartTimer(delay_timer, rtcdrvTimerTypeOneshot, ms, Timer_Expired,
	NULL);
	while (!timer_finished) {
		EMU_EnterEM1();
	}
}

void EM2_Delay(int ms) {
	timer_finished = false;
	RTCDRV_StartTimer(delay_timer, rtcdrvTimerTypeOneshot, ms, Timer_Expired,
	NULL);
	while (!timer_finished) {
		EMU_EnterEM2(false);
	}
}
