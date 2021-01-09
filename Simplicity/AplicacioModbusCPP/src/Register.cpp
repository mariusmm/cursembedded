/*
 * Register.cpp
 *
 *  Created on: 21 de des. 2020
 *      Author: marius
 */

#include <string.h>

#include "em_msc.h"

#include "Register.hpp"

#define USERDATA  ((uint32_t *) USERDATA_BASE)

Register::Register() {
	MSC_Init();

	memcpy(m_data, USERDATA, sizeof(m_data));

	if (m_data[NUM_REGISTER_FILE] != MAGIC_FLASH) {
		memset(m_data, 0, NUM_REGISTER_FILE);
		m_data[NUM_REGISTER_FILE] = MAGIC_FLASH;
	}

	m_permanent[REGISTER_THRES_LOW] = true;
	m_permanent[REGISTER_THRES_HIGH] = true;
	m_permanent[REGISTER_OFFSET] = true;
	m_permanent[REGISTER_DUTYCYLE] = true;
	m_permanent[REGISTER_LED] = true;
	m_permanent[REGISTER_RELAY1] = true;
	m_permanent[REGISTER_RELAY2] = true;
}

bool Register::store(void) {
	MSC_Status_TypeDef ret;
	MSC_ErasePage(USERDATA);
	ret = MSC_WriteWord(USERDATA, m_data, 128);
	if (ret == mscReturnOk) {
		return true;
	} else {
		return false;
	}
}

uint16_t Register::direct_read(unsigned int reg_num) {
	if (reg_num < NUM_REGISTER_FILE) {
		return m_data[reg_num];
	} else {
		return 0xFFFF;
	}
}

void Register::direct_write(unsigned int reg_num, uint16_t data) {
	if (reg_num < NUM_REGISTER_FILE) {
		m_data[reg_num] = data;
	}
}

uint16_t Register::read(unsigned int reg_num) {
	if (m_getter[reg_num] != nullptr) {
		m_getter[reg_num](reg_num);
	}
	return m_data[reg_num];
}

bool Register::write(unsigned int reg_num, uint16_t data) {
	if (reg_num < NUM_REGISTER_FILE) {
		m_data[reg_num] = data;

		if (m_setter[reg_num] != nullptr) {
			m_setter[reg_num]->callback(reg_num, data);
		}
	}

	return m_permanent[reg_num];
}


bool Register::register_wr_callback(unsigned int reg_num, Register_cb *cb) {
	if (reg_num < NUM_REGISTER_FILE) {
			m_setter[reg_num] = cb;
			return true;
		} else {
			return false;
		}
	return true;
}

bool Register::register_rd_callback(unsigned int reg_num, void func(int)) {
		if (reg_num < NUM_REGISTER_FILE) {
			m_getter[reg_num] = func;
			return true;
		} else {
			return false;
		}
}
