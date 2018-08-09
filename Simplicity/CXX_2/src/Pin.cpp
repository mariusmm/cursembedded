/**
 ******************************************************************************
 * @file    Pin.cpp
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-Aug-2018
 * @brief   EFM32 Pin abstraction
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
 */

#include "Pin.h"

namespace EFM32 {

Pin::Pin() {

}

Pin::Pin(GPIO_Port_TypeDef port, unsigned int pin) {
	m_port = port;
	m_pin = pin;
}

Pin::~Pin() {
	// TODO Auto-generated destructor stub
}


GPIO_Port_TypeDef Pin::getPort() {
	return m_port;
}

unsigned int Pin::getNumber() {
	return m_pin;
}

void Pin::setPort(GPIO_Port_TypeDef port) {
	m_port = port;
}
void Pin::setNumber(unsigned int number) {
	m_pin = number;
}

void Pin::redefine(GPIO_Port_TypeDef port, unsigned int number) {
	m_port = port;
	m_pin = number;
}
} /* namespace BSP */
