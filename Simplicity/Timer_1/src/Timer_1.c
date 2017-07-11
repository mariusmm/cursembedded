/**
 ******************************************************************************
 * @file    Timer_1.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-July-2017
 * @brief   Basic Timer management
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

#include "em_device.h"
#include "em_chip.h"
#include "em_timer.h"
#include "em_gpio.h"
#include "em_cmu.h"

#define TOP_VALUE 13671

TIMER_Init_TypeDef timerInit =
  {
    .enable     = false,
    .debugRun   = false,
    .prescale   = timerPrescale1024,
    .clkSel     = timerClkSelHFPerClk,
    .fallAction = timerInputActionNone,
    .riseAction = timerInputActionNone,
    .mode       = timerModeUp,
    .dmaClrAct  = false,
    .quadModeX4 = false,
    .oneShot    = true,
    .sync       = false,
  };


int main(void)
{
  /* Chip errata */
  CHIP_Init();

  /* Enable clks to peripherals */
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_TIMER0, true);

  /* Set I/O */
  GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); 	/* LED */
  GPIO_PinModeSet(gpioPortD, 8, gpioModeInput, 0); 			/* Boto 0 */
  GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 0); 		/* Boto 1 */

  /* Switch off LED */
  GPIO_PinOutClear(gpioPortD, 7);

  /* configure TImer */
  TIMER_Init(TIMER0, &timerInit);

  /* Infinite loop */
  while (1) {

	  /* If user push button 0, start timer 0 */
	  if (GPIO_PinInGet(gpioPortD, 8) == 0) {
		  TIMER_CounterSet(TIMER0, 0);
		  TIMER_Enable(TIMER0, true);
	  }

	  /* If timer count gets to TOP_VALUE, toggle LED and stop Timer */
	  if (TIMER_CounterGet(TIMER0) >= TOP_VALUE) {
		  GPIO_PinOutToggle(gpioPortD, 7);
		  TIMER_Enable(TIMER0, false);
		  TIMER_CounterSet(TIMER0, 0);
	  }
  }
}
