/**
 ******************************************************************************
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @file    PeriodicTasks.c
 * @version V1.0
 * @date    16-January-2019
 * @brief   Periodic tasks example with a simple kernel
 * @license This project is released under the GNU Public License
 *
 *
 * ******************************************************************************
 */

#include "PeriodicTasks.h"

static struct my_timers {
    RTCDRV_TimerID_t timers_array;  /**< Timer ID */
    volatile bool semaphores;       /**< semaphore for "mutex" access */
    mycallback_t callbacks;         /**< Callback to call when timer expires */
    int value;						/**< value to identify which timeout has expired */
} my_timers[EMDRV_RTCDRV_NUM_TIMERS];


static void TimerCallback(RTCDRV_TimerID_t id, void* param) {
	/* timer ’id’ expired, must set semaphore*/
	int index;

	index = *(int*)param;

	my_timers[index].semaphores = true;
}

void RegisterTask(mycallback_t func, uint32_t period) {
	static int i = 0;

	my_timers[i].callbacks = func;
	my_timers[i].value = i;
	RTCDRV_AllocateTimer(&my_timers[i].timers_array);
	RTCDRV_StartTimer(my_timers[i].timers_array, rtcdrvTimerTypePeriodic, period, TimerCallback, &my_timers[i].value);

	i++;
}

void KernelExec() {
	int i;

	for (i = 0; i < EMDRV_RTCDRV_NUM_TIMERS; i++) {
		if (my_timers[i].semaphores == true) {
			my_timers[i].semaphores = false;
			if (my_timers[i].callbacks) {
				my_timers[i].callbacks();
			}
		}
	}
}

void Tasks_Init() {
	int i;

	RTCDRV_Init();
	SLEEP_Init(NULL, NULL);
    SLEEP_SleepBlockBegin(sleepEM2);

	for (i = 0; i < EMDRV_RTCDRV_NUM_TIMERS; i++) {
		my_timers[i].timers_array = 0xFFFFFFFF;
		my_timers[i].semaphores = false;
		my_timers[i].callbacks = NULL;
		my_timers[i].value = 0;
	}
}


