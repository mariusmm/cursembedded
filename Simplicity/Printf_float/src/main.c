/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    02-May-2018
 * @brief   Use of printf with floats
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

#include <stdio.h>

#include "em_device.h"
#include "em_chip.h"

#include "BSP.h"

#define FRACT(x) (x >= 0? ((int)(x*10000)) % 10000 : ((int)(-x*10000)) % 10000)

inline int fract(float x);

int fract(float x) {
	if (x>=0) {
		return ((int)(x*10000)) % 10000;
	} else {
		return ((int)(-x*10000)) % 10000;
	}
}

int main(void)
{

  float value, value2;

  /* Chip errata */
  CHIP_Init();

  setupSWOForPrint();

  value = 5.032;
  value2 = 0;

  /* Infinite loop */
  while (1) {
	  value2 += value;
	  printf("Float value %d.%04d\n", (int)value2, FRACT(value2));
	  printf("Float value bis: %d.%04d\n", (int)value2, fract(value2));
  }
}
