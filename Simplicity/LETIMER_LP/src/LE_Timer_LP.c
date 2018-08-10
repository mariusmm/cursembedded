/**
 ******************************************************************************
 * @file    LE_Timer_LP.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    01-August-2018
 * @brief   Using LETIMER with IRQs and LP Modes to toggle LED every 4 seconds
 * @license This project is released under the GNU Public License.
 *
 * ****************************************************************************
 */

#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_letimer.h"


/* comment this #define out to use Ultra Low Frequency RC Oscillator */
#define USE_ULFRCO

#ifdef USE_ULFRCO
#define PRESCALER cmuClkDiv_1
#define EFECTIVE_CLK_FREQ (1000/PRESCALER)
#else
#define PRESCALER cmuClkDiv_4
#define EFECTIVE_CLK_FREQ (32768/PRESCALER)
#endif

#define SLEEP_SECONDS 4

#define TOP_VALUE (EFECTIVE_CLK_FREQ * SLEEP_SECONDS)

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
#else
	/* LFXO is 32,678 kHz */
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
#endif


	CMU_ClockDivSet(cmuClock_LETIMER0, PRESCALER);

	/* Enable clks to peripherals */
	CMU_ClockEnable(cmuClock_CORELE, true);
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_LETIMER0, true);

	/* Set I/O */
	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */

	/* Switch off LED */
	GPIO_PinOutClear(gpioPortD, 7);

	/* Enable overflow interrupt */
	LETIMER_IntEnable(LETIMER0, LETIMER_IF_UF);

	/* Enable IRQ for Timer 0*/
	NVIC_EnableIRQ(LETIMER0_IRQn);

	/* Configure Timer */
	LETIMER_Init(LETIMER0, &letimerInit);


	LETIMER_CompareSet(LETIMER0, 0, TOP_VALUE);
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
