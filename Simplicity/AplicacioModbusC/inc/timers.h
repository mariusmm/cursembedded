/*
 * timers.h
 *
 *  Created on: 20 de des. 2020
 *      Author: marius
 */

#ifndef INC_TIMERS_H_
#define INC_TIMERS_H_

#include <stdint.h>

void Timers_init(void);
void Timers_setDuty(uint16_t duty);
void testTimer(void);
bool Timers_triggered(void);
void Timers_processed(void);
void DutyUpdate(int);

#endif /* INC_TIMERS_H_ */
