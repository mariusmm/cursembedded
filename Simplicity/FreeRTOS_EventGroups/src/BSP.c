/**
 ******************************************************************************
 * @file    BSP.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    25-July-2017
 * @brief   Basic BSP enabling printf
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

#include "BSP.h"

/**
 * @brief Initialize SWO to be used by printf
 */
static void setupSWOForPrint(void) {
	/* Enable GPIO clock. */
	CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;

	/* Enable Serial wire output pin */
	GPIO->ROUTE |= GPIO_ROUTE_SWOPEN;

#if defined(_EFM32_GIANT_FAMILY) || defined(_EFM32_LEOPARD_FAMILY) || defined(_EFM32_WONDER_FAMILY)
	/* Set location 0 */
	GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK)) | GPIO_ROUTE_SWLOCATION_LOC0;

	/* Enable output on pin - GPIO Port F, Pin 2 */
	GPIO->P[5].MODEL &= ~(_GPIO_P_MODEL_MODE2_MASK);
	GPIO->P[5].MODEL |= GPIO_P_MODEL_MODE2_PUSHPULL;
#else
	/* Set location 1 */
	GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK))
			| GPIO_ROUTE_SWLOCATION_LOC1;
	/* Enable output on pin */
	GPIO->P[2].MODEH &= ~(_GPIO_P_MODEH_MODE15_MASK);
	GPIO->P[2].MODEH |= GPIO_P_MODEH_MODE15_PUSHPULL;
#endif

	/* Enable debug clock AUXHFRCO */
	CMU->OSCENCMD = CMU_OSCENCMD_AUXHFRCOEN;

	/* Wait until clock is ready */
	while (!(CMU->STATUS & CMU_STATUS_AUXHFRCORDY))
		;

	/* Enable trace in core debug */
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	ITM->LAR = 0xC5ACCE55;
	ITM->TER = 0x0;
	ITM->TCR = 0x0;
	TPI->SPPR = 2;
	TPI->ACPR = 0xf;
	ITM->TPR = 0x0;
	DWT->CTRL = 0x400003FE;
	ITM->TCR = 0x0001000D;
	TPI->FFCR = 0x00000100;
	ITM->TER = 0x1;
}

/**
 * @brief write function to be used by printf
 */
int _write(int file, const char *ptr, int len) {
	int x;
	for (x = 0; x < len; x++) {
		ITM_SendChar(*ptr++);
	}
	return (len);
}

/**
 * @brief Init LED GPIO
 */
static void BSP_LedInit(void) {
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */
}

void LedOn(void) {
	GPIO_PinOutSet(gpioPortD, 7);
}

void LedOff(void) {
	GPIO_PinOutClear(gpioPortD, 7);
}

void LedToggle(void) {
	GPIO_PinOutToggle(gpioPortD, 7);
}


void TriggerSet(void) {
	GPIO_PinOutSet(gpioPortD, 6);
}

void TriggerClear(void) {
	GPIO_PinOutClear(gpioPortD, 6);
}

void TriggerToggle(void) {
	GPIO_PinOutToggle(gpioPortD, 6);
}


/**
 * @brief Init buttons GPIOs
 *
 * This init function enables IRQs and sets priority to 6
 */
static void BSP_ButtonsInit() {
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(gpioPortD, 8, gpioModeInput, 0); /* Boto 0 */
	GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 0); /* Boto 1 */

	/* Trigger signal on PD6 */
	GPIO_PinModeSet(gpioPortD, 6, gpioModePushPullDrive, 0);

	/* Set Interrupt configuration for both buttons */
	GPIO_IntConfig(gpioPortD, 8, false, true, true);
	GPIO_IntConfig(gpioPortB, 11, false, true, true);

	/* Set priority to interrupts  */
	NVIC_SetPriority(GPIO_EVEN_IRQn, 6);
	NVIC_SetPriority(GPIO_ODD_IRQn, 6);

	/* Enable interrupts */
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
}

void BSP_Init(void) {
	BSP_LedInit();
	BSP_ButtonsInit();
	setupSWOForPrint();
}
