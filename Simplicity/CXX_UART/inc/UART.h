/**
 ******************************************************************************
 * @file    UART.h
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    06-Aug-2018
 * @brief	C++ based driver for UART
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "em_gpio.h"
#include "em_cmu.h"
#include "em_usart.h"
#include <string>
#include <streambuf>

#include "CircularBuffer.h"

//#define USE_CHAR_OP
//#define USE_STRING_OP

class UART {
public:
	UART(unsigned long baudrate, unsigned int uart_number,
			unsigned int location, bool use_irq);
	virtual ~UART();

	void Send();
	void Tx(unsigned char c) const;

#ifdef USE_CHAR_OP
	UART& operator<<(char* str) {
		for(char* it = str; *it; ++it) {
			this->Tx(*it);
		}
		return *this;
	}
#endif
#ifdef USE_STRING_OP
	UART& operator<<(std::string str) {
		for(std::string::iterator it = str.begin(); it != str.end(); ++it) {
			this->Tx(*it);
		}
		return *this;
	}
#endif

	int AvailableData();
	int GetData();
	void SendData(char c);


	friend void USART1_TX_IRQHandler();
	friend void USART1_RX_IRQHandler();

private:
	void USART1_TX_IRQHandler(void);
	void USART1_RX_IRQHandler(void);

private:
	CircularBuffer m_TX;
	CircularBuffer m_RX;
	USART_TypeDef *m_uart;
};


#endif /* INC_UART_H_ */
