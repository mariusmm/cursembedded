/*
 * Interrupt.hpp
 *
 *  Created on: 8 de gen. 2021
 *      Author: marius
 */

#ifndef SRC_INTERRUPT_HPP_
#define SRC_INTERRUPT_HPP_

// https://www.embedded.com/interrupts-in-c/

#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"

#include <cstdint>
#include <array>


#define MAX_IRQS 2

class Interrupt{
public:
    Interrupt(void) {};
    static void Register(int interrupt_numberber, Interrupt* intThisPtr);

    static void TriggerInterrupt(int num) {
      if (ISRVectorTable[num] != nullptr) {
          ISRVectorTable[num]->ISR();
      }
    }

    virtual ~Interrupt() {};
    virtual void ISR(void) = 0;

private:
    static Interrupt* ISRVectorTable[MAX_IRQS];
};



#endif /* SRC_INTERRUPT_HPP_ */
