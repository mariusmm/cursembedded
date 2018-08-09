/**
 ******************************************************************************
 * @file    CXX_1.cpp
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-Aug-2018
 * @brief   Basic GPIO management with C++ and classes
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
 */

#include "em_device.h"
#include "em_chip.h"

#include "LED.h"
#include "Button.h"

int main(void)
{
  /* Chip errata */
  CHIP_Init();

  BSP::LED my_led;
  BSP::Button button_0(gpioPortD, 8);
  BSP::Button button_1(gpioPortB, 11, false);

  /* Infinite loop */
  while (1) {
	  if (button_0.getValue() == false) {
		  my_led.Off();
	  }

	  if (button_1.getValue() == false) {
		  my_led.On();
	  }
  }
}
