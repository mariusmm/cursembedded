/*
 * RegisterFile.c
 *
 *  Created on: 19 de des. 2020
 *      Author: marius
 */

#include <IODrivers.h>
#include <stdint.h>
#include <string.h>

#include "em_msc.h"

#include "definitions.h"
#include "RegisterFile.h"
#include "ADPS9960.h"
#include "timers.h"

#define USERDATA  ((uint32_t *) USERDATA_BASE)

Registers_t Registers = {0};


void register_init(void) {

	MSC_Init();

	memcpy(Registers.data, USERDATA, sizeof(Registers.data));

	if (Registers.data[NUM_REGISTER_FILE] != MAGIC_FLASH) {
		for (int i = 0; i < NUM_REGISTER_FILE; i++) {
			Registers.data[i] = 0;
		}
		Registers.data[NUM_REGISTER_FILE] = MAGIC_FLASH;
	}

	Registers.permanent[REGISTER_THRES_LOW] = true;
	Registers.permanent[REGISTER_THRES_HIGH] = true;
	Registers.permanent[REGISTER_OFFSET] = true;
	Registers.permanent[REGISTER_DUTYCYLE] = true;
	Registers.permanent[REGISTER_LED] = true;
	Registers.permanent[REGISTER_RELAY1] = true;
	Registers.permanent[REGISTER_RELAY2] = true;

	Registers.getter[REGISTER_DISTANCE] = APDS9960Distance_cb;
	Registers.setter[REGISTER_LED] = LEDUpdate;
	Registers.setter[REGISTER_RELAY1] = RelayUpdate;
	Registers.setter[REGISTER_RELAY2] = RelayUpdate;
	Registers.setter[REGISTER_DUTYCYLE] = DutyUpdate;
}

uint16_t register_direct_read (unsigned int reg_num) {
	if (reg_num < NUM_REGISTER_FILE) {
		return Registers.data[reg_num];
	} else {
		return 0xFFFF;
	}
}

void register_direct_write (unsigned int reg_num, uint16_t data) {
	if (reg_num < NUM_REGISTER_FILE) {
			Registers.data[reg_num] = data;
	}
}

uint16_t register_read(unsigned int reg_num) {

	if (Registers.getter[reg_num] != NULL) {
		Registers.getter[reg_num](reg_num);
	}
	return Registers.data[reg_num];
}

bool register_write(unsigned int reg_num, uint16_t data) {

	if (reg_num < NUM_REGISTER_FILE) {
		Registers.data[reg_num] = data;

		if (Registers.setter[reg_num] != NULL) {
				Registers.setter[reg_num](reg_num);
		}
	}

	return Registers.permanent[reg_num];
}

void register_store(void) {
	MSC_ErasePage(USERDATA);
	MSC_WriteWord(USERDATA, Registers.data, 128);
}
