/*
 * ADPS9960.h
 *
 *  Created on: 20 de des. 2020
 *      Author: marius
 */

#ifndef INC_ADPS9960_H_
#define INC_ADPS9960_H_

bool ADPS9960_Init(void);
uint8_t ADPS9960_GetDistance(void);
void APDS9960Distance_cb(int reg_num);

#endif /* INC_ADPS9960_H_ */
