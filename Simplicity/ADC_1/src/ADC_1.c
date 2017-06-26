#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_timer.h"
#include "em_adc.h"

/**
 * LES_LIGHT_SENSE  PC4 ->
 * LES_LIGHT_EXCITE PD6
 *
 */

#define PWM_FREQ 1000


int main(void) {
	/* Chip errata */
	CHIP_Init();

	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_ADC0, true);

	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */
	GPIO_PinModeSet(gpioPortS, 6, gpioModeInput, 0); /* Excite */
//	GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 0); /* Boto 1 */

	/* Infinite loop */
	while (1) {
		/* nothings to do */
		;
	}
}
