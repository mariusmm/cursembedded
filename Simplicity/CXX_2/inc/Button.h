/*
 * Button.h
 *
 *  Created on: 5 ag. 2018
 *      Author: marius
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "em_gpio.h"
#include "em_cmu.h"

#include "Pin.h"

namespace BSP {

class Button {
public:
	Button(EFM32::Pin aa, bool pull=false, bool pullup=true);
	virtual ~Button();

	bool getValue();


	// overload () operator for simple use of the object
    bool operator()() {
    	return this->getValue();
    }

private:
	EFM32::Pin m_pin;
	bool m_pull;
	bool m_pullup;
};

} /* namespace BSP */

#endif /* BUTTON_H_ */
