/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    04-June-2018
 * @brief   Basic application example
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

/* emlib includes */
#include "em_chip.h"
#include "em_rtc.h"

/* our includes */
#include "BSP.h"
#include "APDS-9960.h"
#include "PWM.h"

volatile bool signal = false;

void RTC_IRQHandler(void) {
	/* Clear interrupt source */
	RTC_IntClear(RTC_IFC_COMP0);

	signal = true;
}

/**
 * @brief main function
 */
int main(void) {
	uint8_t p_data;
	bool ret;
	RTC_Init_TypeDef rtcInit;

	/* Chip errata */
	CHIP_Init();

	/* RTC */
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
	CMU_ClockDivSet(cmuClock_RTC, cmuClkDiv_1);
	CMU_ClockEnable(cmuClock_RTC, true);
	CMU_ClockEnable(cmuClock_HFLE, true );

	rtcInit.enable = true; /* Enable RTC after init has run */
	rtcInit.comp0Top = true; /* Clear counter on compare match */
	rtcInit.debugRun = false; /* Counter shall keep running during debug halt. */

	RTC_CompareSet(0, 16384);
	RTC_IntEnable(RTC_IFC_COMP0);
	NVIC_ClearPendingIRQ(RTC_IRQn);
	NVIC_EnableIRQ(RTC_IRQn);

	/* Initialize the RTC */
	RTC_Init(&rtcInit);

	/* Init functions for our BSP */
	BSP_Init();

	/* Init libraries */
	APDS_9960_Init();

	PWM_Init();

	/* Set 0 % duty cycle by default */
	PWM_Set(0);

	APDS_9960_InitProximity();

	while (true) {

		while(signal == false);

		signal = false;

		ret = APDS_9960_ReadProximity(&p_data);

		if (ret == true) {
			printf("Proximity: %d\r\n", p_data);

			/* Convert from range 0 - 256 to 0 - 100 */
			PWM_Set((p_data * 100) / 256);
		}

	}

	return 0;
}

/**
 * @}
 */

/**
 * @mainpage I2C Application example
 *
 * This example project shows how to get data from a sensor (APDS-9960)
 * and changes the PWM duty cycle (and hence, LED power)
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
