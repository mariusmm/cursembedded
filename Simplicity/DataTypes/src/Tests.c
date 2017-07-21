/**
 ******************************************************************************
 * @file    Tests.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    21-July-2017
 * @brief   Performance test
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
 */

#include "Tests.h"

#define TEST_LOOP 500000

struct example {
	int array[255];
	int some_other_stuff;
};


void Test1(void) {
	int i;
	volatile uint32_t dummy = 0;

	for (i=0;i<TEST_LOOP;i++) {
		dummy++;
	}
}

void Test2(void) {
	int i;
	volatile uint8_t dummy = 0;

	for (i=0;i<TEST_LOOP;i++) {
		dummy++;
	}
}

void Test3(void) {
	int i;
	volatile float dummy = 0.0;

	for (i=0;i<TEST_LOOP;i++) {
		dummy++;
	}
}

int Test4b(struct example param) {
	int i;
	int total = 0;

	for(i=0;i<255;i++) {
		total += param.array[i];
	}

	return total;
}

void Test4(void) {
	volatile int a;
	struct example b;
	int i;
	for (i=0;i<TEST_LOOP/100;i++) {
		a += Test4b(b);
	}
}


int Test5b(struct example *param) {
	int i;
	int total = 0;

	for(i=0;i<255;i++) {
		total += param->array[i];
	}

	return total;
}

void Test5(void) {
	volatile int a;
	struct example b;
	int i;
	for (i=0;i<TEST_LOOP/100;i++) {
		a += Test5b(&b);
	}
}
