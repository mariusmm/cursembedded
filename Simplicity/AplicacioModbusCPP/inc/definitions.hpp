/*
 * definitions.h
 *
 *  Created on: 19 de des. 2020
 *      Author: marius
 */

#ifndef INC_DEFINITIONS_HPP_
#define INC_DEFINITIONS_HPP_


#define MODBUS_SLAVE_BUFFER_SIZE (256)
#define SYSTICKS_PER_SECOND (1000)

#define MODBUS_SLAVE_ADDRRESS (10)

#define NUM_REGISTER_FILE (10)

#define REGISTER_DISTANCE (0)
#define REGISTER_THRES_LOW (1)
#define REGISTER_THRES_HIGH (2)
#define REGISTER_OFFSET (3)
#define REGISTER_DUTYCYLE (4)
#define REGISTER_DOREAD (6)
#define REGISTER_LED (7)
#define REGISTER_RELAY1 (8)
#define REGISTER_RELAY2 (9)

#define MAGIC_FLASH (0xCAFE)

#endif /* INC_DEFINITIONS_HPP_ */
