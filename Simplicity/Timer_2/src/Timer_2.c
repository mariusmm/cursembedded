/**
 ******************************************************************************
 * @file    Timer_2.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-July-2017
 * @brief   Using timer with IRQs
 * @license This project is released under the GNU Public License.
 *
 * ******************************************************************************
 */

#include "em_device.h"
#include "em_chip.h"
#include "em_timer.h"
#include "em_gpio.h"
#include "em_cmu.h"

#define TOP_VALUE 13671

void TIMER0_IRQHandler(void) {
	uint32_t flags;

	/* Clear flag for TIMER0 */
	flags = TIMER_IntGet(TIMER0);
	TIMER_IntClear(TIMER0, flags);

	/* Toggle LED ON/OFF */
	GPIO_PinOutToggle(gpioPortD, 7);
}

void GPIO_EVEN_IRQHandler(void) {
	uint32_t flags;

	/* clear flags */
	flags = GPIO_IntGet();
	GPIO_IntClear(flags);

	/* Set counter to 0  */
	TIMER_CounterSet(TIMER0, 0);

	/* Set TIMER Top value */
	TIMER_TopSet(TIMER0, TOP_VALUE);

	/* Start Timer */
	TIMER_Enable(TIMER0, true);
}

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


int main(void) {
	/* Chip errata */
	CHIP_Init();

	/* Enable clks to peripherals */
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_TIMER0, true);

	/* Set I/O */
	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */
	GPIO_PinModeSet(gpioPortD, 8, gpioModeInput, 1); 		 /* Boto 0 */
	GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 0); 		 /* Boto 1 */

	/* Set Interrupt configuration for button 0 */
	GPIO_IntConfig(gpioPortD, 8, false, true, true);
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);

	/* Switch off LED */
	GPIO_PinOutClear(gpioPortD, 7);

	/* Enable overflow interrupt */
	TIMER_IntEnable(TIMER0, TIMER_IF_OF);

	/* Enable IRQ for Timer 0*/
	NVIC_EnableIRQ(TIMER0_IRQn);

	/* Configure Timer */
	TIMER_Init(TIMER0, &timerInit);

	/* Infinite loop */
	while (1) {
		/* nothing to do here */
	}
}
