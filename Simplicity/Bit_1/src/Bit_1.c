/**
 ******************************************************************************
 * @file    Bit_1.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    02-August-2018
 * @brief   Example manipulating individuals bits in C
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

#include <stdio.h>

#include "em_device.h"
#include "em_chip.h"

#include "BSP.h"


int main(void)
{
  volatile uint8_t my_value;

  /* Chip errata */
  CHIP_Init();

  setupSWOForPrint();

  uint8_t my_variable;

  my_variable = 5;
  // Set to '1' bit 4
  my_variable |= (1 << 4);
  printf("my_variable: 0x%02X\r\n", my_variable);
  // Now set to '0' bit 2
  my_variable &= ~(1 << 2);
  printf("my_variable: 0x%02X\r\n", my_variable);
  // Now we toggle bit 0 twice
  my_variable ^= (1 << 0);
  printf("my_variable: 0x%02X\r\n", my_variable);
  my_variable ^= (1 << 0);
  printf("my_variable: 0x%02X\r\n", my_variable);

  /* Infinite loop */
  while (1) {
	;
  }
}
