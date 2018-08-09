/**
 ******************************************************************************
 * @file    LED.h
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-Aug-2018
 * @brief   Basic LED managemtn class
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
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
