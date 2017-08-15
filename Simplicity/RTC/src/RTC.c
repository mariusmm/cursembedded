/**
 ******************************************************************************
 * @file    RTC.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    15-August-2017
 * @brief   RTC example
 * @license This project is released under the GNU Public License.
 *
 * This example shows how to use RTC peripheral to generate a IRQ every X seconds.
 * ******************************************************************************
 */

#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_rtc.h"

void GPIO_EVEN_IRQHandler(void) {
	uint32_t aux;

	aux = GPIO_IntGet();

	/* clear flags */
	GPIO_IntClear(aux);
}

void RTC_IRQHandler(void) {
	/* Clear interrupt source */
	RTC_IntClear(RTC_IFC_COMP0);

	GPIO_PinOutToggle(gpioPortD, 7);
}

int main(void) {

	RTC_Init_TypeDef rtcInit;

	/* Chip errata */
	CHIP_Init();

	CMU_OscillatorEnable(cmuSelect_LFXO, true, true);

	CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */
	GPIO_PinModeSet(gpioPortD, 8, gpioModeInput, 0); /* Boto 0 */

	/* Set Interrupt configuration for both buttons */
	GPIO_IntConfig(gpioPortD, 8, false, true, true);

	/* Enable interrupts */
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);

	CMU_ClockSelectSet( cmuClock_LFA, cmuSelect_LFXO );
	CMU_ClockDivSet( cmuClock_RTC, cmuClkDiv_32768 );
	CMU_ClockEnable(cmuClock_RTC, true);
	CMU_ClockEnable(cmuClock_HFLE, true );

	/* RTC settings */
	rtcInit.enable = true; /* Enable RTC after init has run */
	rtcInit.comp0Top = true; /* Clear counter on compare match */
	rtcInit.debugRun = false; /* Counter shall keep running during debug halt. */

	RTC_CompareSet(0, 3);

	/* Enabling Interrupt from RTC */
	RTC_IntEnable(RTC_IFC_COMP0);
	NVIC_EnableIRQ(RTC_IRQn);

	/* Initialize the RTC */
	RTC_Init(&rtcInit);

	/* Infinite loop */
	while (1) {
		;
	}
}
