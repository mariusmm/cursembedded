/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    17-April-2019
 * @brief   Shows FreeRTOS Event Groups mechanism
 * @license This project is released under the GNU Public License.
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
#include "event_groups.h"

/* emlib includes */
#include "em_chip.h"

/* emdrv includes */
#include "sleep.h"

/* our includes */
#include "BSP.h"

/** Priority for the ToogleLed Task*/
#define TOGGLE_TASK_PRIORITY          (tskIDLE_PRIORITY + 1)

#define FIRST_BUTTON_BIT (1 << 0)
#define SECOND_BUTTON_BIT (1 << 1)

EventGroupHandle_t event_group;

/**
 * @brief Button 0 ISR
 *
 * Signals task @ref TaskCalculateDelay every time button 0 is pushed
 */
void GPIO_EVEN_IRQHandler(void) {
	uint32_t aux;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	/* clear flags */
	aux = GPIO_IntGet();
	GPIO_IntClear(aux);

	xEventGroupSetBitsFromISR(event_group, FIRST_BUTTON_BIT, &xHigherPriorityTaskWoken);

	/* Awake a task ? */
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * @brief Button 1 ISR
 *
 * Unused
 */
void GPIO_ODD_IRQHandler(void) {
	uint32_t aux;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	/* clear flags */
	aux = GPIO_IntGet();
	GPIO_IntClear(aux);

	xEventGroupSetBitsFromISR(event_group, SECOND_BUTTON_BIT, &xHigherPriorityTaskWoken);

	/* Awake a task ? */
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}



/**
 * @brief
 *
 * @param pParameter Unused parameter
 * @return none
 *
 */
static void TaskLedToggle(void *pParameter) {
	(void) pParameter;

	while (true) {
		xEventGroupWaitBits(event_group, (FIRST_BUTTON_BIT | SECOND_BUTTON_BIT), pdTRUE, pdTRUE, portMAX_DELAY);
		TriggerToggle();
		LedToggle();
	}
}

/**
 * @brief main function
 */
int main(void) {
	/* Chip errata */
	CHIP_Init();

	/* Init functions for our BSP */
	BSP_Init();

	/* Init SLEEP library */
	SLEEP_Init(NULL, NULL);
#if (configSLEEP_MODE < 3)
	SLEEP_SleepBlockBegin((SLEEP_EnergyMode_t) (configSLEEP_MODE + 1));
#endif


	event_group = xEventGroupCreate();

	if (event_group != NULL) {
		/* Create tasks */
		xTaskCreate(TaskLedToggle, (const char *) "LedToggle",
		configMINIMAL_STACK_SIZE, NULL, TOGGLE_TASK_PRIORITY, NULL);

		/* Start FreeRTOS Scheduler */
		vTaskStartScheduler();

	}
	return 0;
}

/**
 * @}
 */

