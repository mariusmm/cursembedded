/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    25-July-2017
 * @brief   Basic FreeRTOS Mutex example
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
#include "semphr.h"

/* emlib includes */
#include "em_chip.h"

/* emdrv includes */
#include "sleep.h"

/* our includes */
#include "BSP.h"
#include "APDS-9960.h"
#include "PWM.h"

/** Priority for the task #1 */
#define TASK_1_PRIORITY          (tskIDLE_PRIORITY + 1)

/** Priority for the task #2 */
#define TASK_2_PRIORITY          (tskIDLE_PRIORITY + 1)

/** Queue to send data from task #1 to Task #2 */
QueueHandle_t sensor_data_queue;

/**
 * @brief Reads the proximity data value and sends it by a queue
 *
 * @param pParameter Unused parameter, declared due to FreeRTOS API
 * @return none
 *
 */
static void ReadSensor_task(void *pParameter) {
	uint8_t p_data;
	bool ret;

	(void) pParameter;

	APDS_9960_InitProximity();

	while (pdTRUE) {
		ret = APDS_9960_ReadProximity(&p_data);

		if (ret == true) {
			xQueueSend(sensor_data_queue, &p_data, portMAX_DELAY);
		}

		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}

/**
 * @brief Prints proximity value and sets PWM value proportionally to this value
 *
 * @param pParameter Unused parameter, declared due to FreeRTOS API
 * @return none
 *
 */
static void MngData_task(void *pParameter) {

	uint8_t p_data;
	(void) pParameter;

	while (pdTRUE) {
		xQueueReceive(sensor_data_queue, &p_data, portMAX_DELAY);
		printf("Proximity: %d\r\n", p_data);

		/* Convert from range 0 - 256 to 0 - 100 */
		PWM_Set((p_data * 100) / 256 );
	}
}

/**
 * @brief main function
 */
int main(void) {
	/* Chip errata */
	CHIP_Init();

	/* Init SLEEP library */
	SLEEP_Init(NULL, NULL);
#if (configSLEEP_MODE < 3)
	SLEEP_SleepBlockBegin((SLEEP_EnergyMode_t) (configSLEEP_MODE + 1));
#endif

	/* Init functions for our BSP */
	BSP_Init();

	/* Init libraries */
	APDS_9960_Init();
	PWM_Init();

	/* Set 0 % duty cycle by default */
	PWM_Set(0);

	/* Create queue to send data between two tasks */
	sensor_data_queue = xQueueCreate(8, sizeof(uint8_t));

	/* Create read sensor task */
	xTaskCreate(ReadSensor_task, (const char *) "ReadSensor",
	configMINIMAL_STACK_SIZE, NULL, TASK_1_PRIORITY, NULL);

	/* Create print & LED ctrl task */
	xTaskCreate(MngData_task, (const char *) "MngData",
	configMINIMAL_STACK_SIZE, NULL, TASK_2_PRIORITY, NULL);

	/* Start FreeRTOS Scheduler */
	vTaskStartScheduler();

	return 0;
}

/**
 * @}
 */

/**
 * @mainpage FreeRTOS I2C Application example with FreeRTOS
 *
 *
 * This example project shows how to get data from a sensor (APDS-9960)
 * and send it to other task.
 *
 * This task changes PWM duty cycle (and hence, LED power)
 * proportionally to the sensor data.
 *
 * This project uses the following libraries:
 * @li @ref BSP
 * @li @ref APDS_9960
 * @li @ref I2C_Wrapper
 * @li @ref PWM
 *
 * @}
 */
