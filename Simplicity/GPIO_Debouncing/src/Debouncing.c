/**
 ******************************************************************************
 * @file    Debouncing.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    14-Nov-2018
 * @brief   GPIO with debouncing example
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */
#include <stdio.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_timer.h"

#include "BSP.h"

volatile uint32_t button_counter = 0;
volatile bool timer_running;

#define DEBOUNCE_VALUE (1367)

void timer_debouce_Init(void) {
	TIMER_Init_TypeDef timerInit ={
	  .enable = false,
	  .debugRun = false,
	  .prescale = timerPrescale1024,
	  .clkSel = timerClkSelHFPerClk,
	  .fallAction = timerInputActionNone,
	  .riseAction = timerInputActionNone,
	  .mode = timerModeUp,
	  .dmaClrAct = false,
	  .quadModeX4 = false,
	  .oneShot = true,
	  .sync = false };

	CMU_ClockEnable(cmuClock_TIMER1, true);

	TIMER_IntEnable(TIMER1, TIMER_IF_OF);
	NVIC_EnableIRQ(TIMER1_IRQn);

	TIMER_TopSet(TIMER1, DEBOUNCE_VALUE);

	TIMER_Init(TIMER1, &timerInit);

	timer_running = false;
}


void TIMER1_IRQHandler(void) {
 uint32_t flags;

 /* Clear flag for TIMER1 */
 flags = TIMER_IntGet(TIMER1);
 TIMER_IntClear(TIMER1, flags);

 timer_running = false;

 if (GPIO_PinInGet(gpioPortD, 8) == 1) {
	 button_counter++;
 }
}

void GPIO_EVEN_IRQHandler(void) {
	uint32_t aux;

	aux = GPIO_IntGet();

	/* clear flags */
	GPIO_IntClear(aux);

	/* Set LED off */
	GPIO_PinOutClear(gpioPortD, 7);

	if (timer_running == false) {
	   timer_running = true;
	   TIMER_TopSet(TIMER1, DEBOUNCE_VALUE);
	   TIMER_Enable(TIMER1, true);
	}

}

void GPIO_ODD_IRQHandler(void) {
	uint32_t aux;

	aux = GPIO_IntGet();

	/* clear flags */
	GPIO_IntClear(aux);

	/* Set LED on */
	GPIO_PinOutSet(gpioPortD, 7);
}

int main(void) {
	/* Chip errata */
	CHIP_Init();


	setupSWOForPrint();

	CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_PinModeSet(gpioPortD,  7, gpioModePushPullDrive, 0); /* LED */
	GPIO_PinModeSet(gpioPortD,  8, gpioModeInput, 0); /* Boto 0 */
	GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 0); /* Boto 1 */

	/* Set Interrupt configuration for both buttons */
	GPIO_IntConfig(gpioPortD,  8, false, true, true);
	GPIO_IntConfig(gpioPortB, 11, false, true, true);

	/* Enable interrupts */
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);

	/* Infinite loop */
	while (1) {
		/* nothings to do*/
		printf("value: %ld\r\n", button_counter);
	}
}
