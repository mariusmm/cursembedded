/**
 ******************************************************************************
 * @file    FLASH_1.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    09-August-2018
 * @brief   Basic FLASH example
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

#include <string.h>
#include <stdio.h>
#include "em_device.h"
#include "em_msc.h"
#include "BSP.h"

uint32_t * userDataPage = (uint32_t *) USERDATA_BASE;

typedef struct {
	uint32_t field1;
	uint8_t  field2;
	uint32_t field3;
	bool	 field4;
} persistent_data_t;

persistent_data_t my_persistent_data;

void Flash_Write() {
	MSC_ErasePage(userDataPage);
	MSC_WriteWord(userDataPage, &my_persistent_data,
	                       sizeof(my_persistent_data));

	printf("Data stored in Flash\r\n");
}

void Flash_Read() {
    memcpy(&my_persistent_data, userDataPage, sizeof(my_persistent_data));

}

void Print_data() {
	printf("Persistent data: ");
	printf("field 1: %lu\t", my_persistent_data.field1);
	printf("field 2: %u\t", my_persistent_data.field2);
	printf("field 3: %lu\t", my_persistent_data.field3);
	if (my_persistent_data.field4 == true) {
		printf("field 4: true\r\n");
	} else {
		printf("field 4: false\r\n");
	}
}

int main(void) {
	/* Chip errata */
	CHIP_Init();

	/* BSP Init */
	BSP_Init();

	printf("size: %d bytes\r\n", sizeof(my_persistent_data));

	/* Flash controller Init */
    MSC_Init();

    Flash_Read();
    Print_data();

	/* Infinite loop */
	while (1) {
		if (GPIO_PinInGet(gpioPortD, 8) == 0) {
			Flash_Write();
			LedOff();
		}

		if (GPIO_PinInGet(gpioPortB, 11) == 0) {
			my_persistent_data.field1++;
			my_persistent_data.field2 = 127;
			my_persistent_data.field3 = 1714;
			my_persistent_data.field4 = !my_persistent_data.field4;
			Print_data();
			LedOn();
		}

	}
}
