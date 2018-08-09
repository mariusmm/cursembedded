/**
 ******************************************************************************
 * @file    LED.cpp
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-Aug-2018
 * @brief   Basic LED management class
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
 */

#include "LED.h"

namespace BSP {

LED::LED() {

	CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */
}

LED::~LED() {
	// TODO Auto-generated destructor stub
}


void LED::On() {
	GPIO_PinOutSet(gpioPortD, 7);
}

void LED::Off() {
	GPIO_PinOutClear(gpioPortD, 7);
}

void LED::Toggle() {
	GPIO_PinOutToggle(gpioPortD, 7);
}

} /* namespace BSP */
