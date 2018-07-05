/**
 ******************************************************************************
 * @file    ADC_1_LP.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-July-2018
 * @brief   ADC example with LP
 * @license This project is released under the GNU Public License
 *
 * PD6 --> ADC0_CH6 input
 *
 * ******************************************************************************
 */

#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_adc.h"
#include "em_emu.h"

#include "BSP.h"

/**
 * @brief ADC0 ISR
 */
void ADC0_IRQHandler(void) {
	ADC_IntClear(ADC0, ADC_IF_SINGLE);
}


static void ADCConfig(void) {
	ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
	ADC_InitSingle_TypeDef singleInit = ADC_INITSINGLE_DEFAULT;
	singleInit.acqTime = adcAcqTime256;

	/* default options are OK */
	ADC_Init(ADC0, &init);

	/* Select right input channel and reference */
	singleInit.reference = adcRefVDD;
	singleInit.input = adcSingleInpCh6;

	ADC_IntEnable(ADC0, ADC_IEN_SINGLE);
	NVIC_EnableIRQ(ADC0_IRQn);

	ADC_InitSingle(ADC0, &singleInit);
}

int main(void) {
	volatile uint32_t ADCvalue;

	/* Chip errata */
	CHIP_Init();

	setupSWOForPrint();

	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_ADC0, true);

	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */
	GPIO_PinModeSet(gpioPortD, 8, gpioModeInput, 0); /* Boto 0 */
	GPIO_PinModeSet(gpioPortC, 5, gpioModePushPullDrive, 0); /* To measure times */

	ADCConfig();

	/* Infinite loop */
	while (1) {

		ADC_Start(ADC0, adcStartSingle);
		GPIO_PinOutSet(gpioPortC, 5);

		EMU_EnterEM1();

		GPIO_PinOutClear(gpioPortC, 5);

		ADCvalue = ADC_DataSingleGet(ADC0);
		printf("ADC Value %lu\r\n", ADCvalue);
	}
}
