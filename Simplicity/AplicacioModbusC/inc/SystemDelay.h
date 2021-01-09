/*
 * SystemDelay.h
 *
 *  Created on: 19 de des. 2020
 *      Author: marius
 */

#ifndef INC_SYSTEMDELAY_H_
#define INC_SYSTEMDELAY_H_

void SysTicksInit(void);
void SysTicksDelay (uint32_t delay);
void SysTicksDisable(void);
void SysTicksEnable(void);

#endif /* INC_SYSTEMDELAY_H_ */
