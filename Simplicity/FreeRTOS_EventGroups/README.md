FreeRTOS second example
=======================

This examples uses a queue to send data from one task to another.
The example also shows how to send data from a ISR to one of the tasks.

The ISR is attached to button 0

The first task @ref TaskCalculateDelay receives signal (@ref semaphore_button_0) from ISR and increases
delay time. This time is sent to Task 2 using the queue @ref queue_tasks.

The second task (@ref TaskLedToggle) tries to read from the queue, and if there is data,
it waits for the received amount of time (delay) to toggle the LED

This project uses the following libraries:
@li @ref BSP


