/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    2-July-2018
 * @brief   Basic Systick example
 * @license This project is released under the GNU Public License.
 * @note	Example taken from https://www.silabs.com/community/blog.entry.html/2015/06/29/chapter_5_mcu_clocki-pEjC
 * ******************************************************************************
 */

#include <stdint.h>
#include <stdbool.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "BSP.h"

volatile uint32_t msTicks; /* counts 1ms timeTicks */

void Delay(uint32_t dlyTicks);

/**************************************************************************//**
 * @brief SysTick_Handler
 * Interrupt Service Routine for system tick counter
 *****************************************************************************/
void SysTick_Handler(void)
{
  msTicks++;       /* increment counter necessary in Delay()*/
}

/**************************************************************************//**
 * @brief Delays number of msTick Systicks (typically 1 ms)
 * @param dlyTicks Number of ticks to delay
 *****************************************************************************/
void Delay(uint32_t dlyTicks)
{
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) ;
}

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
  /* Chip errata */
  CHIP_Init();

  BSP_Init();

  /* Setup SysTick Timer for 1 msec interrupts  */
  SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000);

  /* Infinite blink loop */
  while (1)
  {
	LedToggle();
    Delay(100);
  }
}
