/**
 ******************************************************************************
 * @file    LE_Timer_LP.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    01-August-2018
 * @brief   Using LETIMER with IRQs and LP Modes
 * @license This project is released under the GNU Public License.
 *
 * ******************************************************************************
 */

#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_letimer.h"


/* comment this #define out to use Ultra Low Freqüency RC Oscillator */
//#define USE_ULFRCO

void LETIMER0_IRQHandler(void) {
	uint32_t flags;

	/* Clear flag for LETIMER0 */
	flags = LETIMER_IntGet(LETIMER0);
	LETIMER_IntClear(LETIMER0, flags);

	/* Toggle LED ON/OFF */
	GPIO_PinOutToggle(gpioPortD, 7);
}

LETIMER_Init_TypeDef letimerInit =
  {
	.enable     = false,
	.debugRun   = false,
	.rtcComp0Enable = true,
	.rtcComp1Enable = false,
	.comp0Top = true,
	.bufTop = false,
	.out0Pol = 0,
	.out1Pol = 0,
	.ufoa0 = _LETIMER_CTRL_UFOA0_NONE,
	.ufoa1 = _LETIMER_CTRL_UFOA0_NONE,
	.repMode = letimerRepeatFree
  };


int main(void) {
	/* Chip errata */
	CHIP_Init();

#ifdef USE_ULFRCO
	/* ULFRCO is 1,000 kHz */
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);

	/* Divided by 1 -> 1,000 kHz */
	CMU_ClockDivSet(cmuClock_LETIMER0, cmuClkDiv_1);
#else
	/* LFXO is 32,678 kHz */
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

	/* Divided by 2^2 (=4) -> 32,768/4 = 8192 kHz */
	CMU_ClockDivSet(cmuClock_LETIMER0, cmuClkDiv_4);
#endif

	/* Enable clks to peripherals */
	CMU_ClockEnable(cmuClock_CORELE, true);
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_LETIMER0, true);

	/* Set I/O */
	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */

	/* Switch off LED */
	GPIO_PinOutClear(gpioPortD, 7);

	/* Enable overflow interrupt */
	LETIMER_IntEnable(LETIMER0, LETIMER_IF_COMP0);

	/* Enable IRQ for Timer 0*/
	NVIC_EnableIRQ(LETIMER0_IRQn);

	/* Configure Timer */
	LETIMER_Init(LETIMER0, &letimerInit);


#ifdef USE_ULFRCO
	LETIMER_CompareSet(LETIMER0, 0, 1000*4);
#else
	LETIMER_CompareSet(LETIMER0, 0, 8192*4);
#endif

	LETIMER_Enable(LETIMER0, true);

	/* Infinite loop */
	while (1) {
		/* nothing to do here */
#ifdef USE_ULFRCO
		EMU_EnterEM3(true);
#else
		EMU_EnterEM2(true);
#endif
	}
}
