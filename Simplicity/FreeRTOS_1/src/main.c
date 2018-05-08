/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    6-September-2017
 * @brief   FreeRTOS example
 * @license This project is released under the GNU Public License.
 *
 * ******************************************************************************
 */

/** @addtogroup main
 * @brief main file
 *
 * This example shows how to communicate two tasks using a queue
 * @{
 */

/* STD libraries*/
#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS includes */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* emlib includes */
#include "em_chip.h"

/* emdrv includes */
#include "sleep.h"

/* our includes */
#include "BSP.h"

/** Priority for the ToogleLed Task*/
#define TOGGLE_TASK_PRIORITY          (tskIDLE_PRIORITY + 1)

/** Length of the queue */
#define QUEUE_LENGTH (10)


/** @brief Queue to send & receive ADC values */
QueueHandle_t adc_queue;


/**
 * @brief PWM Ctrl from value received through a queue
 *
 * @param pParameter Unused parameter
 * @return none
 *
 */
static void TaskPWMCtrl(void *pParameter) {

	(void) pParameter;
	uint32_t my_pwm;
	uint32_t recv_pwm;
	uint32_t pwm_value;

	PWMConfig();

	for (;;) {
		if (xQueueReceive(adc_queue, &recv_pwm, portMAX_DELAY)) {
			my_pwm = recv_pwm;

			/*
			 * Value received from ADC tssk has a range from 0 to 4095 (12 bits)
			 * We may map these values to a 0 to PWM_FREQ in 6 steps
			 */
			if (my_pwm > PWM_FREQ) {
				my_pwm = PWM_FREQ;
			}

			pwm_value = my_pwm;

			TIMER_CompareBufSet(TIMER1, 1, pwm_value);
		}
	}
}

/**
 * @brief Get ADC value and sent it through queue
 *
 * @param pParameter Unused parameter
 * @return none
 *
 */
static void TaskADCRead(void *pParameter) {

	(void) pParameter;
	uint32_t ADCvalue;

	ADCConfig();

	while (1) {
		ADC_Start(ADC0, adcStartSingle);

		while (ADC0->STATUS & ADC_STATUS_SINGLEACT);

		ADCvalue = ADC_DataSingleGet(ADC0);
		xQueueSend(adc_queue, &ADCvalue, portMAX_DELAY);

		vTaskDelay( pdMS_TO_TICKS(250));
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

	/* Create Queue */
	adc_queue = xQueueCreate(QUEUE_LENGTH, sizeof(uint32_t));

	/* Create our first task */
	xTaskCreate(TaskPWMCtrl, (const char *) "PWMCtrl",
	configMINIMAL_STACK_SIZE, NULL, TOGGLE_TASK_PRIORITY, NULL);

	xTaskCreate(TaskADCRead, (const char *) "ADCRead",
	configMINIMAL_STACK_SIZE, NULL, TOGGLE_TASK_PRIORITY, NULL);

	/* Start FreeRTOS Scheduler */
	vTaskStartScheduler();

	return 0;
}

/**
 * @}
 */

/**
 * @}
 */
