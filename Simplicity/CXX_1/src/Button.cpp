/**
 ******************************************************************************
 * @file    Button.cpp
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-Aug-2018
 * @brief   Basic Button managemtn class
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
 */

#include "Button.h"

namespace BSP {

Button::Button(GPIO_Port_TypeDef port, unsigned int pin, bool pull, bool pullup) {
	CMU_ClockEnable(cmuClock_GPIO, true);

	m_port = port;
	m_pin = pin;
	m_pull = pull;
	m_pullup = pullup;

	if (m_pull == false) {
	GPIO_PinModeSet(port, pin, gpioModeInput, 0);
	} else {
		if (m_pullup == true) {
			GPIO_PinModeSet(port, pin, gpioModeInputPull, 1);
		} else {
			GPIO_PinModeSet(port, pin, gpioModeInputPull, 0);
		}
	}

}

Button::~Button() {
	// TODO Auto-generated destructor stub
}

bool Button::getValue() {
	unsigned int pin_value;

	pin_value = GPIO_PinInGet(m_port, m_pin);
	if (pin_value == 0) {
		return false;
	} else {
		return true;
	}
}

} /* namespace BSP */
