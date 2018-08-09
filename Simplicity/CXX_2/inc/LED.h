/*
 * LED.h
 *
 *  Created on: 5 ag. 2018
 *      Author: marius
 */

#ifndef LED_H_
#define LED_H_

#include "em_gpio.h"
#include "em_cmu.h"


namespace BSP {

class LED {
public:
	LED();
	virtual ~LED();

	void On();
	void Off();
	void Toggle();

};

} /* namespace BSP */

#endif /* LED_H_ */
