/*
 * RegisterFile.h
 *
 *  Created on: 19 de des. 2020
 *      Author: marius
 */

#ifndef INC_REGISTERFILE_H_
#define INC_REGISTERFILE_H_

#include <stdint.h>
#include <stdbool.h>

#include "definitions.h"

typedef struct {
	uint16_t data[NUM_REGISTER_FILE + 1];
	void (*getter[NUM_REGISTER_FILE])(int);
	void (*setter[NUM_REGISTER_FILE])(int);
	bool permanent[NUM_REGISTER_FILE];
} Registers_t;

void register_init(void);
void register_store(void);

uint16_t register_direct_read (unsigned int reg_num);
void register_direct_write (unsigned int reg_num, uint16_t data);

uint16_t register_read(unsigned int reg_num);
bool register_write(unsigned int reg_num, uint16_t data);

#endif /* INC_REGISTERFILE_H_ */
