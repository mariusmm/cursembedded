/*
 * UART.hpp
 *
 *  Created on: 29 de des. 2020
 *      Author: marius
 */

#ifndef INC_UART_HPP_
#define INC_UART_HPP_

#include <cstdint>

#include "em_device.h"
#include "em_cmu.h"
#include "em_leuart.h"
#include "em_usart.h"

#include "definitions.hpp"
#include "SystemDelay.hpp"

enum uart_peripheral {
	LEUARTA, LEUARTB, UARTA, UARTB, UARTC, UARTD, UARTE,
};

class SerialUART {
public:
	virtual ~SerialUART() =default;
	virtual void send(const uint8_t *const pkt, int len) = 0;
	virtual int16_t RxTimeout() = 0;
};

template<uart_peripheral device, int location>
class UART : public SerialUART {
public:
	UART() {
		CMU_ClockEnable(cmuClock_GPIO, true);

		if constexpr (device == LEUARTA) {
			CMU_ClockEnable(cmuClock_CORELE, true);
#ifdef LEUART1
		} else if constexpr (device == LEUARTB)  {
			CMU_ClockEnable(cmuClock_CORELE, true);
#endif
		} else if constexpr (device == UARTA)  {
			CMU_ClockEnable(cmuClock_USART0, true);
		}

		EnableDevice();
		InitializeDevice();
	}

	int16_t RxTimeout() {
		bool data = false;
		int16_t c = -1;
		int retries = 0;

		do {
			bool status = false;
			if constexpr ((device == LEUARTA) || (device == LEUARTB)) {
				status = getDeviceLEUART()->STATUS & LEUART_STATUS_RXDATAV;
			} else {
				status = getDeviceUSART()->STATUS & USART_STATUS_RXDATAV;
			}

			if (status != 0) {
				data = true;
				if constexpr ((device == LEUARTA) || (device == LEUARTB)) {
					c = (uint16_t) getDeviceLEUART()->RXDATA;
				} else if constexpr (device == UARTA) {
					c = (uint16_t) getDeviceUSART()->RXDATA;
				}
				break;
			}
			retries++;
			SysTick_mng.Delay(1);
		} while ((data == false) && (retries < 50));

		return c;
	}

	void send(const uint8_t *const pkt, int len) {
		for (int i = 0; i < len; i++) {
			if constexpr ((device == LEUARTA) || (device == LEUARTB)) {
				LEUART_Tx(getDeviceLEUART(), pkt[i]);
			} else {
				USART_Tx(getDeviceUSART(), pkt[i]);
			}
		}
	}

private:
	USART_TypeDef* getDeviceUSART() {
		switch (device) {
		case UARTA:
			return USART0;
		case UARTB:
			return USART1;
#ifdef USART2
		case UARTC:
			return USART2;
#endif
		default:
			return USART0;
		}
	}

	LEUART_TypeDef* getDeviceLEUART() {
		switch (device) {
		case LEUARTA:
			return LEUART0;
#ifdef LEUART1
		case LEUARTB:
			return LEUART1;
#endif
		default:
			return LEUART0;
		}
	}

	constexpr void EnableDevice () {
		switch (device) {
		case LEUARTA:
			CMU_ClockEnable(cmuClock_LEUART0, true);
			break;
#ifdef LEUART1
		case LEUARTB:
			CMU_ClockEnable(cmuClock_LEUART1, true);
			break;
#endif
		case UARTA:
			CMU_ClockEnable(cmuClock_USART0, true);
			break;
		case UARTB:
			CMU_ClockEnable(cmuClock_USART1, true);
			break;
#ifdef USART2
		case UARTC:
			CMU_ClockEnable(cmuClock_USART2, true);
			break;
#endif
		default:
			return;
		}
	}

	constexpr void InitializeDevice() {
		switch (device) {
		case LEUARTA:
		case LEUARTB: {
			GPIO_PinModeSet(gpioPortD, 4, gpioModePushPull, 1);  /* TX Pin */
			GPIO_PinModeSet(gpioPortD, 5, gpioModeInputPull, 1); /* RX Pin */

			LEUART_Init_TypeDef leuart_cfg = LEUART_INIT_DEFAULT;
			LEUART_Init(getDeviceLEUART(), &leuart_cfg);
			getDeviceLEUART()->ROUTE = LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN
						| getLocationLE();
			LEUART_IntEnable(getDeviceLEUART(), LEUART_IEN_RXDATAV);
			NVIC_ClearPendingIRQ(getIRQn());
			NVIC_EnableIRQ(getIRQn());
		}
		break;
		case UARTA:
		case UARTB: {
			GPIO_PinModeSet(gpioPortD, 4, gpioModePushPull, 1);  /* TX Pin */
			GPIO_PinModeSet(gpioPortD, 5, gpioModeInputPull, 1); /* RX Pin */

			USART_InitAsync_TypeDef uart_cfg = USART_INITASYNC_DEFAULT;
			USART_InitAsync(getDeviceUSART(), &uart_cfg);
			getDeviceUSART()->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN
					| getLocation();
			USART_IntEnable(getDeviceUSART(), USART_IEN_RXDATAV);
			NVIC_ClearPendingIRQ(getIRQn());
			NVIC_EnableIRQ(getIRQn());
		}
		return;

		default:
			return;
		}
	}

	constexpr IRQn_Type getIRQn() {
		switch (device) {
		case LEUARTA:
			return LEUART0_IRQn;
		case LEUARTB:
#ifdef LEUART1
			return LEUART1_IRQn;
#else
			return NonMaskableInt_IRQn;
#endif
		case UARTA:
			return USART0_RX_IRQn;
		case UARTB:
			return USART1_RX_IRQn;
		default:
			return NonMaskableInt_IRQn;
		}
	}

	constexpr uint32_t getLocationLE() {
		switch (location) {
			case 0: return LEUART_ROUTE_LOCATION_LOC0;
			case 1: return LEUART_ROUTE_LOCATION_LOC1;
			case 2: return LEUART_ROUTE_LOCATION_LOC2;
			case 3: return LEUART_ROUTE_LOCATION_LOC3;
			case 4: return LEUART_ROUTE_LOCATION_LOC4;
			default:
				return LEUART_ROUTE_LOCATION_DEFAULT;
			}
	}

	constexpr uint32_t getLocation() {
		switch (location) {
			case 0: return USART_ROUTE_LOCATION_LOC0;
			case 1: return USART_ROUTE_LOCATION_LOC1;
			case 2: return USART_ROUTE_LOCATION_LOC2;
			case 3: return USART_ROUTE_LOCATION_LOC3;
			case 4: return USART_ROUTE_LOCATION_LOC4;
			default:
				return USART_ROUTE_LOCATION_DEFAULT;
			}
	}

};

void LEUART0_IRQHandler(void) {
	uint32_t flags;
	flags = LEUART_IntGet(LEUART0);
	LEUART_IntClear(LEUART0, flags);
	LEUART_IntDisable(LEUART0, LEUART_IEN_RXDATAV);
}

#endif /* INC_UART_HPP_ */


