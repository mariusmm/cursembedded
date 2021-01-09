/*
 * IODrivers.h
 *
 *  Created on: 19 de des. 2020
 *      Author: marius
 */

#ifndef INC_IODRIVERS_H_
#define INC_IODRIVERS_H_

void Drivers_init(void);
void LEDOn(void);
void LEDOff(void);

void RelayOn(int relay_num);
void RelayOff(int relay_num);

void LEDUpdate(int value);
void RelayUpdate(int value);

#endif /* INC_IODRIVERS_H_ */
