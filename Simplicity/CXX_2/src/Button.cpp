/**
 ******************************************************************************
 * @file    Button.cpp
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-Aug-2018
 * @brief   More complex GPIO management with C++ and classes
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
 */

#include "Button.h"

namespace BSP {

Button::Button(EFM32::Pin pin, bool pull, bool pullup) {

	CMU_ClockEnable(cmuClock_GPIO, true);

	m_pin = pin;
	m_pull = pull;
	m_pullup = pullup;

	if (m_pull == false) {
	GPIO_PinModeSet(m_pin.getPort(), m_pin.getNumber(), gpioModeInput, 0);
	} else {
		if (m_pullup == true) {
			GPIO_PinModeSet(m_pin.getPort(), m_pin.getNumber(), gpioModeInputPull, 1);
		} else {
			GPIO_PinModeSet(m_pin.getPort(), m_pin.getNumber(), gpioModeInputPull, 0);
		}
	}
}

Button::~Button() {
	// TODO Auto-generated destructor stub
}

bool Button::getValue() {
	unsigned int pin_value;

	pin_value = GPIO_PinInGet(m_pin.getPort(), m_pin.getNumber());
	if (pin_value == 0) {
		return false;
	} else {
		return true;
	}
}

} /* namespace BSP */
