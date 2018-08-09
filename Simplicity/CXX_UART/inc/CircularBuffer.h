/**
 ******************************************************************************
 * @file    CircularBuffer.h
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    06-Aug-2018
 * @brief	Circular Buffer class
 * @license This project is released under the GNU Public License.
 * ******************************************************************************
 */

#ifndef INC_CIRCULARBUFFER_H_
#define INC_CIRCULARBUFFER_H_

#define CIRCULAR_BUFFER_SIZE 64

class CircularBuffer {
public:
	CircularBuffer();
	virtual ~CircularBuffer();

	void PushData(char data);
	int PopData();
	int AvailableData();

private:
	char m_buffer[CIRCULAR_BUFFER_SIZE]; /**< buffer for the stream */
	int m_tail; /**< tail index */
	int m_head; /**< head index */
	int m_overflow; /**< overflow flag */
};


#endif /* INC_CIRCULARBUFFER_H_ */
