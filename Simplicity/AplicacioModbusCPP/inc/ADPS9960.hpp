/*
 * ADPS9960.h
 *
 *  Created on: 24 de des. 2020
 *      Author: marius
 */

#ifndef INC_ADPS9960_HPP_
#define INC_ADPS9960_HPP_

#include <cstdint>

#include <I2C.hpp>

template<i2c_device dev, i2c_location loc, uint8_t sensor_addr>
class ADPS9960 {
public:
	ADPS9960() {

		uint8_t ret_value;

		ret_value = m_i2c.ReadRegister(sensor_addr, 0x92);

		if (ret_value != ID_VALUE) {
			return;
		}

		/* Enable Proximity detection */
		/* ENABLE <- 5 & 2 & 0 bits */
		m_i2c.WriteRegister(sensor_addr, APDS_ENABLE_REG, 0x25);

		/* LED Strength to 100mA, Proximity Gain control to 8x */
		m_i2c.WriteRegister(sensor_addr, APDS_CTRL_1_REG, 0x0C);

		/* LED_BOOST 300% 0111_0001 */
		m_i2c.WriteRegister(sensor_addr, APDS_CTRL_2_REG, 0x71);
	}

	uint8_t GetDistance(void) {
		uint8_t aux;
		uint8_t status;

		do {
			status = m_i2c.ReadRegister(sensor_addr, APDS_STATUS_REG);
		} while (status == 0x00);

		aux = m_i2c.ReadRegister(sensor_addr, APDS_PDATA_REG);
		return aux;
	}

private:

	const uint8_t APDS_ENABLE_REG {0x80};
	const uint8_t APDS_CTRL_1_REG {0x8F};
	const uint8_t APDS_CTRL_2_REG {0x90};
	const uint8_t APDS_STATUS_REG {0x93};
	const uint8_t APDS_PDATA_REG  {0x9C};
	const uint8_t ID_VALUE 		  {0xAB};
	I2C<dev, loc> m_i2c;
};



#endif /* INC_ADPS9960_HPP_ */
