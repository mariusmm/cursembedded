/*
 * IODrivers.c
 *
 *  Created on: 19 de des. 2020
 *      Author: marius
 */

#include "em_gpio.h"

#include "IODrivers.h"
#include "RegisterFile.h"
#include "definitions.h"

extern Registers_t Registers;

void Drivers_init(void) {
	GPIO_PinModeSet (gpioPortC, 4, gpioModePushPull, 0);
	GPIO_PinModeSet (gpioPortC, 5, gpioModePushPull, 0);
	GPIO_PinModeSet (gpioPortC, 12, gpioModePushPull, 0);
}

void LEDOn() {
	GPIO_PinOutSet(gpioPortC, 12);
}

void LEDOff() {
	GPIO_PinOutClear(gpioPortC, 12);
}

void RelayOn(int relay_num) {
	if (relay_num == 0) {
		GPIO_PinOutSet(gpioPortC, 4);
	} else {
		GPIO_PinOutSet(gpioPortC, 5);
	}
}

void RelayOff(int relay_num) {
	if (relay_num == 0) {
		GPIO_PinOutClear(gpioPortC, 4);
	} else {
		GPIO_PinOutClear(gpioPortC, 5);
	}
}


void LEDUpdate(int reg_num) {

	int value = Registers.data[reg_num];

	if (value == 0) {
		LEDOff();
	} else {
		LEDOn();
	}
}

void RelayUpdate(int reg_num) {

	int value = Registers.data[reg_num];

	if (value == 0) {
		RelayOff(reg_num - REGISTER_RELAY1);
	} else {
		RelayOn(reg_num - REGISTER_RELAY1);
	}
}
