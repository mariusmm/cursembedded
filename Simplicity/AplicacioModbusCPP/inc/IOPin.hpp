/*
 * IOPin.hpp
 *
 *  Created on: 21st Dec. 2020
 *      Author: Màrius Montón
 */

#ifndef INC_IOPINTEMPL_HPP_
#define INC_IOPINTEMPL_HPP_

#include "em_cmu.h"
#include "em_gpio.h"

#include "Interrupt.hpp"

namespace IOPin {

enum gpio_port {
  PORTA, PORTB, PORTC, PORTD, PORTE, PORTF,
};

enum gpio_pin {
  PIN0 = 0,
  PIN1,
  PIN2,
  PIN3,
  PIN4,
  PIN5,
  PIN6,
  PIN7,
  PIN8,
  PIN9,
  PIN10,
  PIN11,
  PIN12,
  PIN13,
  PIN14,
  PIN15,
  PIN16,
  PIN17,
  PIN18,
  PIN19,
  PIN20,
  PIN21,
  PIN22,
  PIN23,
  PIN24,
  PIN25,
  PIN26,
  PIN27,
  PIN28,
  PIN29,
  PIN30,
  PIN31
};

enum gpio_type {
  PIN_IN, PIN_OUT, PIN_OC, PIN_I2C
};

enum irq_enable {
  IRQ_ENABLE, IRQ_DISABLE
};

enum irq_mode  {
  IRQ_RISING, IRQ_FALLING, IRQ_BOTH
};

}

class PINBase {
public:
  virtual ~PINBase() {};
  virtual void Set() = 0;
  virtual void Reset() = 0;
  virtual bool Get() = 0;
  virtual void Toggle() = 0;
};

typedef void(*isr_cb)(int);

template<IOPin::gpio_port port, IOPin::gpio_pin pin, IOPin::gpio_type type,
         IOPin::irq_enable irq_enable = IOPin::IRQ_DISABLE,
         IOPin::irq_mode irq_mode = IOPin::IRQ_RISING,
         isr_cb isr = nullptr>
class PIN  : public PINBase , public Interrupt {
public:

  PIN() {
    CMU_ClockEnable(cmuClock_GPIO, true);
    GPIO_PinModeSet(EFM32TranslatePort(port), EFM32TranslatePin(pin), EFM32TranslateType(type), 0);

    if constexpr(irq_enable == IOPin::IRQ_ENABLE) {
        Interrupt::Register( EFM32TranslatePin(pin), this);

        if constexpr(irq_mode == IOPin::IRQ_RISING) {
            GPIO_IntConfig(EFM32TranslatePort(port), EFM32TranslatePin(pin), true, false, true);
        } else if constexpr(irq_mode == IOPin::IRQ_FALLING) {
            GPIO_IntConfig(EFM32TranslatePort(port), EFM32TranslatePin(pin), false, true, true);
        } else {
            GPIO_IntConfig(EFM32TranslatePort(port), EFM32TranslatePin(pin), true, true, true);
        }

        if constexpr (pin % 2) {
            NVIC_EnableIRQ(GPIO_ODD_IRQn);
        } else {
            NVIC_EnableIRQ(GPIO_EVEN_IRQn);
        }
    }

  }

  virtual void ISR() {
    if constexpr(isr != nullptr) {
        isr(pin);
    }
  };

  constexpr void Set() {
    GPIO_PinOutSet(EFM32TranslatePort(port), EFM32TranslatePin(pin));
  }

  constexpr void Reset() {
    GPIO_PinOutClear(EFM32TranslatePort(port), EFM32TranslatePin(pin));
  }

  constexpr bool Get() {
    return GPIO_PinInGet(EFM32TranslatePort(port), EFM32TranslatePin(pin));
  }

  constexpr void Toggle() {
    GPIO_PinOutToggle(EFM32TranslatePort(port), EFM32TranslatePin(pin));
  }

private:

  constexpr GPIO_Port_TypeDef EFM32TranslatePort(IOPin::gpio_port p) {
    switch (p) {
    case IOPin::PORTA:
      return gpioPortA;
    case IOPin::PORTB:
      return gpioPortB;
    case IOPin::PORTC:
      return gpioPortC;
    case IOPin::PORTD:
      return gpioPortD;
    case IOPin::PORTE:
      return gpioPortE;
    case IOPin::PORTF:
      return gpioPortF;
    default:
      return gpioPortF;
    }
  }

  constexpr unsigned int EFM32TranslatePin(IOPin::gpio_pin p) {
    return static_cast<unsigned int>(p);
  }

  constexpr GPIO_Mode_TypeDef EFM32TranslateType(IOPin::gpio_type p) {
    switch (p) {
    case IOPin::PIN_IN:
      return gpioModeInput;
    case IOPin::PIN_OUT:
      return gpioModePushPull;
    case IOPin::PIN_OC:
      return gpioModeWiredAndDrive;
    case IOPin::PIN_I2C:
      return gpioModeWiredAnd;
    default:
      return gpioModeDisabled;
    }
  }
};

#endif /* INC_IOPINTEMPL_HPP_ */
