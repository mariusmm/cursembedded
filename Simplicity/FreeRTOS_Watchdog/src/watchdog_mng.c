/**
 ******************************************************************************
 * @file    watchdog_mng.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-September-2018
 * @brief   Watchdog manager for FreeRTOS
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

#include "watchdog_mng.h"

/**
 * @brief variable to store touch of all tasks
 */
static volatile uint8_t watchdog_list;

/**
 * mutex to protect watchdog_list variable
 */
static SemaphoreHandle_t watchdog_mutex;

void watchdogTouch(uint8_t task) {
	xSemaphoreTake(watchdog_mutex, portMAX_DELAY);
	watchdog_list |= task;
	xSemaphoreGive(watchdog_mutex);
}

static void watchdogClear() {
	xSemaphoreTake(watchdog_mutex, portMAX_DELAY);
	watchdog_list = 0;
	xSemaphoreGive(watchdog_mutex);
}

/**
 * @brief task that manage watchdog
 * @param parameter mandatory, unusued
 */
static void watchdogTask(void *parameter) {

	while (1) {
		if (watchdog_list == WATCHDOG_FULL) {
			WDOG_Feed();
			watchdogClear();
		}

		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

bool watchdogInit() {

	BaseType_t ret_status;

	WDOG_Init_TypeDef init = {
	.enable = true, /* Start watchdog when init done */
	.debugRun = false, /* WDOG not counting during debug halt */
	.em2Run = true, /* WDOG counting when in EM2 */
	.em3Run = true, /* WDOG counting when in EM3 */
	.em4Block = false, /* EM4 can be entered */
	.swoscBlock = false, /* Do not block disabling LFRCO/LFXO in CMU */
	.lock = false, /* Do not lock WDOG configuration (if locked, reset needed to unlock) */
	.clkSel = wdogClkSelULFRCO, /* Select 1kHZ WDOG oscillator */
	.perSel = wdogPeriod_2k, /* Set the watchdog period to 2049 clock periods (ie ~2 seconds)*/
	};

	CMU_ClockEnable(cmuClock_HFLE, true);

	watchdog_list = 0;

	WDOG_Init(&init);

	watchdog_mutex = xSemaphoreCreateMutex();

	ret_status = xTaskCreate(watchdogTask, (const char *) "WDTTAsk",
			configMINIMAL_STACK_SIZE, NULL, WDT_TASK_PRIORITY, NULL);

	if (ret_status != pdPASS) {
		return false;
	} else {
		return true;
	}

}
