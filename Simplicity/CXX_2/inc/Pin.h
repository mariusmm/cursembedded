/*
 * Pin.h
 *
 *  Created on: 5 ag. 2018
 *      Author: marius
 */

#ifndef PIN_H_
#define PIN_H_

#include "em_gpio.h"

namespace EFM32 {

class Pin {
public:
	Pin();
	Pin(GPIO_Port_TypeDef port, unsigned int pin);
	virtual ~Pin();
	GPIO_Port_TypeDef getPort();
	unsigned int getNumber();
	void setPort(GPIO_Port_TypeDef port);
	void setNumber(unsigned int number);
	void redefine(GPIO_Port_TypeDef port, unsigned int number);
private:
	GPIO_Port_TypeDef m_port;
	unsigned int m_pin;
};

} /* namespace BSP */

#endif /* PIN_H_ */
