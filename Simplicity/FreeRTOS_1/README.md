FreeRTOS first example
=======================

This examples uses a queue to send data from one task to another.

One task (@ref TaskADCRead) manages the ADC and keeps reading the value in polling mode.
It sends the read value to the other task using the queue @ref adc_queue.

The second task (@ref TaskPWMCtrl) tries to read from the queue, and if there is data,
changes the PWM duty cycle for the board LED.

This project uses the following libraries:
@li @ref BSP


