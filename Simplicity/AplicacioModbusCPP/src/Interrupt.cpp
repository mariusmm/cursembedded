/*
 * Interrupt.cpp
 *
 *  Created on: 8 de gen. 2021
 *      Author: marius
 */


#include "Interrupt.hpp"
#include <bitset>

Interrupt* Interrupt::ISRVectorTable[MAX_IRQS] = {nullptr};

void Interrupt::Register(int interrupt_number, Interrupt* intThisPtr)
{
    ISRVectorTable[interrupt_number] = intThisPtr;
}

void GPIO_EVEN_IRQHandler (void) {
  uint32_t aux = GPIO_IntGet();
  GPIO_IntClear(aux);

  std::bitset<16> pin = aux;
  // no iterator for std::bitset, so old-style for used
  for (std::size_t i = 0; i < pin.size(); i++) {
      if (pin[i] == true) {
          Interrupt::TriggerInterrupt(i);
      }
  }
}

void GPIO_ODD_IRQHandler (void) {
  uint32_t aux = GPIO_IntGet();
  GPIO_IntClear(aux);

  std::bitset<16> pin = aux;
  for (std::size_t i = 0; i < pin.size(); i++) {
      if (pin[i] == true) {
          Interrupt::TriggerInterrupt(i);
      }
  }
}
