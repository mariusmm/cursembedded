/**
 ******************************************************************************
 * @file    CircularBuffer2.h
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    2-March-2018
 * @brief   CIrcularBuffer implementation
 * @license This project is released under the GNU Public License.
 *
 * ******************************************************************************
 */

#ifndef INC_CIRCULARBUFFER_H_
#define INC_CIRCULARBUFFER_H_

#include <stdint.h>

#define CIRCULAR_BUFFER_SIZE 64

typedef struct {
	char buffer[CIRCULAR_BUFFER_SIZE]; /**< buffer for the stream */
	int tail; /**< tail index */
	int head; /**< head index */
	int overflow; /**< overflow flag */
} CircularBuffer_t;


/**
 * @brief Inser data in the circularbuffer
 * @param data byte to store
 * @param buffer CircularBuffer to use
 */
void PushData(uint8_t data, CircularBuffer_t* buffer);

/**
 * @brief Retrieve data from circularBuffer
 * @param buffer CircularBuffer to use
 * @return byte retrieved or -1 if buffer is empty
 */
int PopData(CircularBuffer_t* buffer);

/**
 * @brief Returns number of data available on circularBuffer
 * @param buffer CircularBuffer to use
 * @returns bytes availables
 */
int AvailableData(CircularBuffer_t* buffer);

#endif /* INC_CIRCULARBUFFER_H_ */
