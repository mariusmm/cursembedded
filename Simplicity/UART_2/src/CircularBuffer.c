/**
 ******************************************************************************
 * @file    CircularBuffer2.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    2-March-2018
 * @brief   CIrcularBuffer implementation
 * @license This project is released under the GNU Public License.
 *
 * ******************************************************************************
 */

#include "CircularBuffer.h"

void PushData(uint8_t data, CircularBuffer_t* buffer) {
	int next;
	int index;

	next = buffer->head + 1;
	if (next >= CIRCULAR_BUFFER_SIZE) {
		next = 0;
	}

	if (next == buffer->tail) {
		buffer->overflow++;
		return;
	} else {
		index = buffer->head;
		buffer->buffer[index] = data;
		buffer->head = next;
	}
}

int PopData(CircularBuffer_t* buffer) {
	int next;
	int ret;
	int index;

	if (buffer->head == buffer->tail) {
		return -1;
	}

	index = buffer->tail;
	ret = buffer->buffer[index];
	next = buffer->tail + 1;

	if (next >= CIRCULAR_BUFFER_SIZE) {
		next = 0;
	}

	buffer->tail = next;

	return ret;
}

int AvailableData(CircularBuffer_t* buffer) {
	int available;

	available = buffer->head - buffer->tail;
	if (available < 0) {
		available = CIRCULAR_BUFFER_SIZE + available;
	}

	return available;
}
