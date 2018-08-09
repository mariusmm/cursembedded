/**
 ******************************************************************************
 * @file    Button.h
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-Aug-2018
 * @brief   Basic Button managemtn class
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "em_gpio.h"
#include "em_cmu.h"

namespace BSP {

class Button {
public:
	Button(GPIO_Port_TypeDef port, unsigned int pin, bool pull=false, bool pullup=true);
	virtual ~Button();

	bool getValue();

private:
	GPIO_Port_TypeDef m_port;
	unsigned int m_pin;
	bool m_pull;
	bool m_pullup;
};

} /* namespace BSP */

#endif /* BUTTON_H_ */
