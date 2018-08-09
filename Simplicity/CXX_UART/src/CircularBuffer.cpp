/**
 ******************************************************************************
 * @file    CircularBuffer.cpp
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    06-Aug-2018
 * @brief	Circular Buffer class
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
 */

#include "CircularBuffer.h"

CircularBuffer::CircularBuffer() {
	// TODO Auto-generated constructor stub
	m_tail = 0;
	m_head = 0;
	m_overflow = 0;
}

CircularBuffer::~CircularBuffer() {
	// TODO Auto-generated destructor stub
}

void CircularBuffer::PushData(char data) {
	int next;
	int index;

	next = m_head + 1;
	if (next >= CIRCULAR_BUFFER_SIZE) {
		next = 0;
	}

	if (next == m_tail) {
		m_overflow++;
		return;
	} else {
		index = m_head;
		m_buffer[index] = data;
		m_head = next;
	}
}

int CircularBuffer::PopData() {
	int next;
	int ret;
	int index;

	if (m_head == m_tail) {
		return -1;
	}

	index = m_tail;
	ret = m_buffer[index];
	next = m_tail + 1;

	if (next >= CIRCULAR_BUFFER_SIZE) {
		next = 0;
	}

	m_tail = next;

	return ret;
}

int CircularBuffer::AvailableData() {
	int available;

	available = m_head - m_tail;
	if (available < 0) {
		available = CIRCULAR_BUFFER_SIZE + available;
	}

	return available;
}

