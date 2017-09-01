/**
 ******************************************************************************
 * @file    ADC_1.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-July-2017
 * @brief   ADC example
 * @license This project is released under the GNU Public License
 * ******************************************************************************
 */

#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_adc.h"

#include "BSP.h"

/* PD6 --> ADC0_CH6 input */

static void ADCConfig(void) {
	ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
	ADC_InitSingle_TypeDef singleInit = ADC_INITSINGLE_DEFAULT;

	/* default options are OK */
	ADC_Init(ADC0, &init);

	/* Select right input channel and reference */
	singleInit.reference = adcRefVDD;
	singleInit.input = adcSingleInpCh6;

	ADC_InitSingle(ADC0, &singleInit);
}

int main(void) {
	uint32_t ADCvalue;

	/* Chip errata */
	CHIP_Init();

	setupSWOForPrint();

	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_ADC0, true);

	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */
	GPIO_PinModeSet(gpioPortD, 8, gpioModeInput, 0); /* Boto 0 */
//	GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 0); /* Boto 1 */

	ADCConfig();

	/* Infinite loop */
	while (1) {
		ADC_Start(ADC0, adcStartSingle);
		while (ADC0->STATUS & ADC_STATUS_SINGLEACT)
			;
		ADCvalue = ADC_DataSingleGet(ADC0);
		printf("ADC Value %lu\r\n", ADCvalue);
	}
}
