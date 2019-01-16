/**
 ******************************************************************************
 * @file    FSM_1.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    07-January-2019
 * @brief   FSM example emulating a very basic thermostat
 * @license This project is released under the GNU Public License
 *
 * PD6 --> ADC0_CH6 input
 *
 * ******************************************************************************
 */

#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_adc.h"

#include "BSP.h"

#define MIN_TEMPERATURE (21)
#define MAX_TEMPERATURE (23)

static void ADCConfig(void) {
	ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
	ADC_InitSingle_TypeDef singleInit = ADC_INITSINGLE_DEFAULT;

	CMU_ClockEnable(cmuClock_ADC0, true);

	/* default options are OK */
	ADC_Init(ADC0, &init);

	/* Select right input channel and reference */
	singleInit.reference = adcRefVDD;
	singleInit.input = adcSingleInpCh6;

	ADC_InitSingle(ADC0, &singleInit);
}

static uint32_t ADCGetValue() {
	uint32_t ADCvalue = 0;

	ADC_Start(ADC0, adcStartSingle);
	while (ADC0->STATUS & ADC_STATUS_SINGLEACT)
		;
	ADCvalue = ADC_DataSingleGet(ADC0);

	return ADCvalue;
}

static uint32_t getTemperature() {
	uint32_t raw_sensor_value;
	uint32_t temperature_value;

	raw_sensor_value = ADCGetValue();

	/* Fake conversion, just for the example we map 0..4095 values of the ADC to 15 .. 35 celsius degree */
	temperature_value = ((raw_sensor_value * 20) / 4095) + 15;

	return temperature_value;
}

static void SwitchOn() {
	LedOn();
}

static void SwitchOff() {
	LedOff();
}

int main(void) {
	uint32_t temperature;

	enum {
		Thermo_ON, Thermo_OFF
	} state;

	/* Chip errata */
	CHIP_Init();

	BSP_Init();
	ADCConfig();

	state = Thermo_OFF;

	/* Infinite loop */
	while (1) {
		temperature = getTemperature();

		switch (state) {
		case Thermo_OFF:
			if (temperature < MIN_TEMPERATURE) {
				state = Thermo_ON;
				printf("Temp: %ld. Changing state to Thermo_ON\r\n",
						temperature);
			}

			SwitchOff();
			break;
		case Thermo_ON:
			if (temperature > MAX_TEMPERATURE) {
				state = Thermo_OFF;
				printf("Temp: %ld. Changing state to Thermo_OFF\r\n",
						temperature);
			}

			SwitchOn();
			break;
		default:
			state = Thermo_OFF;
			SwitchOff();
		}
	}
}
