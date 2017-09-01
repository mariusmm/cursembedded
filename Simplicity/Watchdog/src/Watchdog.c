/**
 ******************************************************************************
 * @file    Watchdog.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    15-August-2017
 * @brief   Watchdog example
 * @license This project is released under the GNU Public License.
 *
 * The example enables watchdog but doesn't feed it in the main loop.
 * To feed the watchdog button PB0 must be pressed.
 * ******************************************************************************
 */

#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_wdog.h"
#include "em_rmu.h"

void GPIO_EVEN_IRQHandler(void) {
	uint32_t aux;

	aux = GPIO_IntGet();

	/* clear flags */
	GPIO_IntClear(aux);

	/* Feed watchdog */
	WDOG_Feed();
}

WDOG_Init_TypeDef init = { .enable = true, /* Start watchdog when init done */
.debugRun = false, /* WDOG not counting during debug halt */
.em2Run = true, /* WDOG counting when in EM2 */
.em3Run = true, /* WDOG counting when in EM3 */
.em4Block = false, /* EM4 can be entered */
.swoscBlock = false, /* Do not block disabling LFRCO/LFXO in CMU */
.lock = false, /* Do not lock WDOG configuration (if locked, reset needed to unlock) */
.clkSel = wdogClkSelULFRCO, /* Select 1kHZ WDOG oscillator */
.perSel = wdogPeriod_4k, /* Set the watchdog period to 2049 clock periods (ie ~4 seconds)*/
};

int main(void) {
	volatile uint32_t i;
	uint32_t resetCause;
	bool resetbyWatchdog;

	/* Chip errata */
	CHIP_Init();

	/* Check what caused last reset */
	resetCause = RMU_ResetCauseGet();
	RMU_ResetCauseClear();

	if (resetCause & RMU_RSTCAUSE_WDOGRST) {
		resetbyWatchdog = true;
	} else {
		resetbyWatchdog = false;
	}

	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_HFLE, true);

	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */
	GPIO_PinModeSet(gpioPortD, 8, gpioModeInput, 0); /* Boto 0 */

	/* Set Interrupt configuration for both buttons */
	GPIO_IntConfig(gpioPortD, 8, false, true, true);

	/* Enable interrupts */
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);

	/* Enable watchdog */
	WDOG_Init(&init);

	/* Infinite loop */
	while (1) {

		if (resetbyWatchdog == true) {
			GPIO_PinOutSet(gpioPortD, 7);
		} else {
			/* Blink LED */
			GPIO_PinOutToggle(gpioPortD, 7);
			for (i = 0; i < 100000; i++)
				;
		}
	}
}
