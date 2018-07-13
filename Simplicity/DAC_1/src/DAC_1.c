/**
 ******************************************************************************
 * @file    DAC_1.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    13-July-2018
 * @brief   DAC example
 * @license This project is released under the GNU Public License
 * ******************************************************************************
 */

#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_dac.h"

#include "BSP.h"

#define DAC_STEP 0x100

volatile bool signal = false;
volatile int32_t DACvalue = 0x00;

void GPIO_EVEN_IRQHandler(void) {
	uint32_t aux;

	aux = GPIO_IntGet();

	/* clear flags */
	GPIO_IntClear(aux);


	DACvalue += DAC_STEP;
	if (DACvalue >= 0x1000) {
		DACvalue = 0x0FFF;
	}
	signal = true;
}

void GPIO_ODD_IRQHandler(void) {
	uint32_t aux;

	aux = GPIO_IntGet();

	/* clear flags */
	GPIO_IntClear(aux);


	DACvalue -= DAC_STEP;
	if (DACvalue < 0) {
		DACvalue = 0;
	}

	signal = true;
}

/* (PB12 - DAC0_OUT1) */

static void DACConfig(void) {
	  /* Use default settings */
	  DAC_Init_TypeDef        init        = DAC_INIT_DEFAULT;
	  DAC_InitChannel_TypeDef initChannel = DAC_INITCHANNEL_DEFAULT;

	  /* Enable the DAC clock */
	  CMU_ClockEnable(cmuClock_DAC0, true);

	  /* Calculate the DAC clock prescaler value that will result in a DAC clock
	   * close to 500kHz. Second parameter is zero, if the HFPERCLK value is 0, the
	   * function will check what the current value actually is. */
	  init.prescale = DAC_PrescaleCalc(500000, 0);

	  /* Set reference voltage to Vdd */
	  init.reference = dacRefVDD;

	  /* Initialize the DAC and DAC channel #1 */
	  DAC_Init(DAC0, &init);
	  DAC_InitChannel(DAC0, &initChannel, 1);
}

int main(void) {


	/* Chip errata */
	CHIP_Init();

	setupSWOForPrint();

	CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */
	GPIO_PinModeSet(gpioPortD, 8, gpioModeInput, 0); /* Boto 0 */
	GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 0); /* Boto 1 */

	/* Set Interrupt configuration for both buttons */
	GPIO_IntConfig(gpioPortD,  8, false, true, true);
	GPIO_IntConfig(gpioPortB, 11, false, true, true);

	/* Enable interrupts */
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);

	DACConfig();

	DAC_Enable(DAC0, 1, true);

	/* Infinite loop */
	while (1) {
		DAC_ChannelOutputSet(DAC0, 1, DACvalue);
		printf("DAC Value %lu (0x%06X)\r\n", (uint32_t) DACvalue, (uint32_t) DACvalue);
		while(signal == false);
		signal = false;
	}
}


/**
 * @mainpage DAC Application example
 * This example outputs a constant value using DAC0 channel 1 (PB12 - DAC0_OUT1)
 * The two buttons increments (decrements) output voltage by constant factor
 * Each step is 205 mV (should be 206 mV)
 * @}
 */

