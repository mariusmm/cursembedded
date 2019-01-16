/**
 ******************************************************************************
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @file    FSM_3.c
 * @version V1.0
 * @date    15-January-2019
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
#include "em_rtc.h"

#include "BSP.h"
#include "Delay.h"

#define MIN_TEMPERATURE (21)
#define MAX_TEMPERATURE (23)

uint32_t temperature;
bool termostat_on;
const uint32_t period = 1000; // in milliseconds

enum {
	Thermo_ON, Thermo_OFF
} state;

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

void read_inputs() {
	temperature = getTemperature();
}

void calc_values() {
	switch (state) {
	case Thermo_OFF:
		termostat_on = false;
		break;
	case Thermo_ON:
		termostat_on = true;
		break;
	}
}

void write_outputs() {
	if (termostat_on) {
		SwitchOn();
	} else {
		SwitchOff();
	}
}

void next_estate() {
	switch (state) {
	case Thermo_OFF:
		if (temperature < MIN_TEMPERATURE) {
			state = Thermo_ON;
			printf("Temp: %ld. Changing state to Thermo_ON\r\n", temperature);
		}
		break;
	case Thermo_ON:
		if (temperature > MAX_TEMPERATURE) {
			state = Thermo_OFF;
			printf("Temp: %ld. Changing state to Thermo_OFF\r\n", temperature);
		}
		break;
	}
}

void loop() {
	uint32_t start_time, end_time, iteration_time;

	start_time = RTC_CounterGet();

	read_inputs();
	calc_values();
	write_outputs();
	next_estate();

	end_time = RTC_CounterGet();
	iteration_time = end_time - start_time;

	Delay(period - iteration_time);
}

int main(void) {

	/* Chip errata */
	CHIP_Init();
	BSP_Init();
	Delay_Init();

	ADCConfig();

	state = Thermo_OFF;

	/* Infinite loop */
	while (1) {
		loop();
	}
}
