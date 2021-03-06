/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-July-2017
 * @brief   Use of printf with SWO example
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

#include<stdio.h>

#include "em_device.h"
#include "em_chip.h"

#include "BSP.h"

int main(void)
{
  /* Chip errata */
  CHIP_Init();

  setupSWOForPrint();

  /* Infinite loop */
  while (1) {
	  printf("hello world\n");
  }
}
