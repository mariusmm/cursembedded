/**
 ******************************************************************************
 * @file    CXX_2.cpp
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-Aug-2018
 * @brief   More complex GPIO management with C++ and classes
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
 */

#include "em_device.h"
#include "em_chip.h"

#include "LED.h"
#include "Button.h"
#include "Pin.h"

int main(void)
{
  /* Chip errata */
  CHIP_Init();

  BSP::LED my_led;

  EFM32::Pin my_pin(gpioPortD, 8);
  EFM32::Pin my_pin2(gpioPortB, 11);

  BSP::Button button_0(my_pin);

  my_pin.setPort(gpioPortB);
  my_pin.setNumber(11);

  BSP::Button button_1(my_pin);

  /* Infinite loop */
  while (1) {
	  if (button_0() == false) {
		  my_led.Off();
	  }

	  if (button_1() == false) {
		  my_led.On();
	  }
  }
}
