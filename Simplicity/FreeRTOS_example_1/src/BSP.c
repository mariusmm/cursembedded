/**
 ******************************************************************************
 * @file    BSP.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    25-July-2017
 * @brief   Basic BSP enabling printf
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
 */

#include "BSP.h"


/** value for pwm */
volatile uint32_t pwm_value = PWM_FREQ/6;


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

/**
 * @brief Init buttons GPIOs
 *
 * This init function enables IRQs and sets priority to 6
 */
static void BSP_ButtonsInit() {
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(gpioPortD, 8, gpioModeInput, 0); /* Boto 0 */
	GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 0); /* Boto 1 */
}

void BSP_Init(void) {
	BSP_LedInit();
	BSP_ButtonsInit();
	setupSWOForPrint();
}


/** @cond */
TIMER_InitCC_TypeDef timerCCInit = {
		.eventCtrl = timerEventEveryEdge,
		.edge =	timerEdgeNone,
		.prsSel = timerPRSSELCh0,
		.cufoa = timerOutputActionNone,
		.cofoa = timerOutputActionSet,
		.cmoa = timerOutputActionClear,
		.mode =	timerCCModePWM,
		.filter = false,
		.prsInput = false,
		.coist = false,
		.outInvert = false,
	};

TIMER_Init_TypeDef timerInit = {
		.enable = true,
		.debugRun = true,
		.prescale = timerPrescale256,
		.clkSel = timerClkSelHFPerClk,
		.fallAction = timerInputActionNone,
		.riseAction = timerInputActionNone,
		.mode =	timerModeUp,
		.dmaClrAct = false,
		.quadModeX4 = false,
		.oneShot = false,
		.sync = false,
	};
/** @endcond */

void PWMConfig(void) {
	CMU_ClockEnable(cmuClock_TIMER1, true);

	SLEEP_SleepBlockBegin(sleepEM2);

	TIMER_InitCC(TIMER1, 1, &timerCCInit);
	TIMER1->ROUTE |= (TIMER_ROUTE_CC1PEN | TIMER_ROUTE_LOCATION_LOC4);

	TIMER_TopSet(TIMER1, PWM_FREQ);
	TIMER_CompareBufSet(TIMER1, 1, pwm_value);
	TIMER_Init(TIMER1, &timerInit);
}

void ADCConfig(void) {
	ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
	ADC_InitSingle_TypeDef singleInit = ADC_INITSINGLE_DEFAULT;

	CMU_ClockEnable(cmuClock_ADC0, true);

	SLEEP_SleepBlockBegin(sleepEM2);

	/* default options are OK */
	ADC_Init(ADC0, &init);

	/* Select right input channel and reference */
	singleInit.reference = adcRefVDD;
	singleInit.input = adcSingleInpCh6;

	ADC_InitSingle(ADC0, &singleInit);
}
