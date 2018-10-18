/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-July-2017
 * @brief   Use of structures
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

#include <stdio.h>

#include "em_device.h"
#include "em_chip.h"

#include "BSP.h"

struct {
	uint8_t fieldS1;
	uint16_t fieldS1b;
	uint32_t fieldL1;
	uint32_t fieldL2;
	uint8_t fieldS2;
} unpacket_struct;

struct __attribute__ ((__packed__)) {
	uint8_t fieldS1;
	uint16_t fieldS1b;
	uint32_t fieldL1;
	uint32_t fieldL2;
	uint8_t fieldS2;
} packet_struct;

int main(void)
{
  int i;
  uint8_t *buffer;

  /* Chip errata */
  CHIP_Init();

  setupSWOForPrint();

  /* Show structure size */
  printf("size of structure_A: %d\n", sizeof(unpacket_struct));
  printf("size of structure_A: %d\n", sizeof(packet_struct));

  /* Fill structures with the same data */
  unpacket_struct.fieldS1 = 0xAA;
  unpacket_struct.fieldS1b = 0xCDEF;
  unpacket_struct.fieldL1 = 0xDEADBEEF;
  unpacket_struct.fieldL2 = 0xDEADC0DE;
  unpacket_struct.fieldS2 = 0x55;

  packet_struct.fieldS1 = 0xAA;
  packet_struct.fieldS1b = 0xCDEF;
  packet_struct.fieldL1 = 0xDEADBEEF;
  packet_struct.fieldL2 = 0xDEADC0DE;
  packet_struct.fieldS2 = 0x55;

  /* Show content of each structure byte by byte */

  buffer = (uint8_t*) &unpacket_struct;
  printf("Unpacket structure: \t");
  for(i = 0; i < sizeof(unpacket_struct); i++) {
	  printf("0x%02X, ", buffer[i]);
  }
  printf("\n");

  buffer = (uint8_t*) &packet_struct;
  printf("Packet structure: \t");
  for(i = 0; i < sizeof(packet_struct); i++) {
	  printf("0x%02X, ", buffer[i]);
  }
  printf("\n");

  /* Infinite loop */
  while (1) {
	  ;
  }
}
