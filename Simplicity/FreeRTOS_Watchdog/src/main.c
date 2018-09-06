/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    5-September-2018
 * @brief   Watchdog & FreeRTOS example
 * @license This project is released under the GNU Public License.s
 *
 * This example shows how to use watchdog on a RTOS.
 *
 * ******************************************************************************
 */

/** @addtogroup main
 * @brief main file
 * @{
 */

/* STD libraries*/
#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS includes */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* emlib includes */
#include "em_chip.h"
#include "em_rmu.h"

/* emdrv includes */
#include "sleep.h"

/* our includes */
#include "BSP.h"
#include "watchdog_mng.h"

/** Priority for the ToogleLed Task*/
#define TOGGLE_TASK_PRIORITY          (tskIDLE_PRIORITY + 1)

volatile bool resetbyWatchdog;

/**
 * @brief Toggle LED
 *
 * @param pParameter Unused parameter
 * @return none
 *
 */
static void TaskLedToggle(void *pParameter) {

	(void) pParameter;

	/* Here optional init variables or functions */

	for (;;) {
		LedToggle();
		watchdogTouch(WATCHDOG_TASK1);
		if (resetbyWatchdog == true) {
			vTaskDelay(pdMS_TO_TICKS(250));
		} else {
			vTaskDelay(pdMS_TO_TICKS(2500));
		}
	}
}

/**
 * @brief main function
 */
int main(void) {
	uint32_t resetCause;

	/* Chip errata */
	CHIP_Init();

	/* Init functions for our BSP */
	BSP_Init();

	/* Init SLEEP library */
	SLEEP_Init(NULL, NULL);
#if (configSLEEP_MODE < 3)
	SLEEP_SleepBlockBegin((SLEEP_EnergyMode_t) (configSLEEP_MODE));
#endif

	/* Check what caused last reset */
	resetCause = RMU_ResetCauseGet();
	RMU_ResetCauseClear();

	if (resetCause & RMU_RSTCAUSE_WDOGRST) {
		resetbyWatchdog = true;
	} else {
		resetbyWatchdog = false;
	}

	watchdogInit();

	/* Create our first task */
	xTaskCreate(TaskLedToggle, (const char *) "LedToggle",
			configMINIMAL_STACK_SIZE, NULL, TOGGLE_TASK_PRIORITY, NULL);

	/* Start FreeRTOS Scheduler */
	vTaskStartScheduler();

	return 0;
}

/**
 * @}
 */

/**
 * @mainpage FreeRTOS Watchdog example
 *
 * This examples shows how to use watchdog in a RTOS (FreeRTOS).
 * There is a task watchdogTask() that manages watchdog
 * Other tasks must call watchdogTouch() with the correct ID as parameter
 *
 * This project uses the following libraries:
 * @li @ref BSP
 * @li @ref Watchdoglib
 *
 * @}
 */
