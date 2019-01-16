/**
 ******************************************************************************
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @file    main.c
 * @version V1.0
 * @date    16-January-2019
 * @brief   Periodic tasks example with a simple kernel
 * @license This project is released under the GNU Public License
 *
 *
 * ******************************************************************************
 */

#include <stdio.h>
#include "em_device.h"

#include "BSP.h"
#include "PeriodicTasks.h"


static void Task1(void) {
	printf("Task 1\r\n");
	LedToggle();
}

static void Task2(void) {
	printf("Task 2\r\n");
}

int main(void) {

	/* Chip errata */
	CHIP_Init();
	BSP_Init();

	Tasks_Init();

	RegisterTask(Task1, 250);
	RegisterTask(Task2, 500);

	/* Infinite loop */
	while (1) {
		SLEEP_Sleep();
		KernelExec();
	}
}
