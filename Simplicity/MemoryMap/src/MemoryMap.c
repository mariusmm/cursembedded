#include "em_device.h"
#include "em_chip.h"


#define FLASH_INFO (*(unsigned char *)0x0FE081F8)
#define RAM_INFO (*(unsigned char *)0x0FE081FA)
#define PART_INFO (*(unsigned char *)0x0FE081FE)

volatile uint32_t aux;

int main(void)
{
  /* Chip errata */
  CHIP_Init();

  /* Infinite loop */
  while (1) {

	  aux = FLASH_INFO; /* 32 kB */
	  aux = RAM_INFO; /* 4 kB */
	  aux = PART_INFO; /* 73 = Tiny Gecko */
  }
}
