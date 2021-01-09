/*
 * i2c.hpp
 *
 *  Created on: 24 de des. 2020
 *      Author: marius
 */

#ifndef INC_I2C_HPP_
#define INC_I2C_HPP_

#include "em_cmu.h"
#include "em_gpio.h"
#include "em_i2c.h"

#include "definitions.hpp"
#include "IOPin.hpp"

enum i2c_device {I2C_DEV0, I2C_DEV1, I2C_DEV2};
enum i2c_location {LOC0, LOC1, LOC2, LOC3, LOC4, LOC5, LOC6};

class SerialIntf {
public:
	virtual uint8_t ReadRegister(uint8_t addr, uint8_t reg) = 0;
	virtual bool WriteRegister(uint8_t addr, uint8_t reg, uint8_t data) = 0;
	virtual ~SerialIntf() =default;
};

template<i2c_device dev, i2c_location loc>
class I2C : SerialIntf {
public:
	I2C() : SerialIntf() {
		I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;
		CMU_ClockEnable(cmuClock_GPIO, true);

		CMU_ClockEnable(getClock(dev), true);

		getDevice(dev)->ROUTE = I2C_ROUTE_SDAPEN | I2C_ROUTE_SCLPEN | getLocation(loc);

		I2C_Init(getDevice(dev), &i2cInit);
	}

	~I2C() {};

	uint8_t ReadRegister(uint8_t addr, uint8_t reg) {
		I2C_TransferReturn_TypeDef I2C_Status;
		I2C_TransferSeq_TypeDef seq;
		uint8_t data[2];

		seq.addr = addr;
		seq.flags = I2C_FLAG_WRITE_READ;

		seq.buf[0].data = &reg;
		seq.buf[0].len = 1;
		seq.buf[1].data = data;
		seq.buf[1].len = 1;

		I2C_Status = I2C_TransferInit(I2C0, &seq);

		while (I2C_Status == i2cTransferInProgress) {
			I2C_Status = I2C_Transfer(I2C0);
		}

		if (I2C_Status != i2cTransferDone) {
			return 0;
		}

		return data[0];
	}

	bool WriteRegister(uint8_t addr, uint8_t reg, uint8_t data) {
		I2C_TransferReturn_TypeDef I2C_Status;
		bool ret_value = false;

		I2C_TransferSeq_TypeDef seq;
		uint8_t dataW[2];

		seq.addr = addr;
		seq.flags = I2C_FLAG_WRITE;

		dataW[0] = reg;
		dataW[1] = data;

		seq.buf[0].data = dataW;
		seq.buf[0].len = 2;
		I2C_Status = I2C_TransferInit(I2C0, &seq);

		while (I2C_Status == i2cTransferInProgress) {
			I2C_Status = I2C_Transfer(I2C0);
		}

		if (I2C_Status != i2cTransferDone) {
			ret_value = false;
		} else {
			ret_value = true;
		}

		return ret_value;
	}

private:

    constexpr I2C_TypeDef* getDevice(i2c_device i2c_dev) {
        switch (i2c_dev) {
			case i2c_device::I2C_DEV0 : return I2C0;
#ifdef I2C1
			case i2c_device::I2C_DEV1 : return I2C1;
#endif
#ifdef I2C2
			case i2c_device::I2C_DEV2 : return I2C2;
#endif
			default: return I2C0;
        }
    }

	constexpr uint32_t getLocation(int location) {
		switch (location) {
			case i2c_location::LOC0 : return I2C_ROUTE_LOCATION_LOC0;
			case i2c_location::LOC1: return I2C_ROUTE_LOCATION_LOC1;
			case i2c_location::LOC2: return I2C_ROUTE_LOCATION_LOC2;
			case i2c_location::LOC3: return I2C_ROUTE_LOCATION_LOC3;
			case i2c_location::LOC4: return I2C_ROUTE_LOCATION_LOC4;
			case i2c_location::LOC5: return I2C_ROUTE_LOCATION_LOC5;
			case i2c_location::LOC6: return I2C_ROUTE_LOCATION_LOC6;
			default:
				return I2C_ROUTE_LOCATION_DEFAULT;
		}
	}

	constexpr CMU_Clock_TypeDef getClock(i2c_device i2c_dev) {
		switch (i2c_dev) {
		case i2c_device::I2C_DEV0 : return cmuClock_I2C0;
#ifdef I2C1
		case i2c_device::I2C_DEV1 : return cmuClock_I2C1;
#endif
#ifdef I2C2
		case i2c_device::I2C_DEV2 : return cmuClock_I2C2;
#endif
		default: return cmuClock_I2C0;
		}
	}
};


#endif /* INC_I2C_HPP_ */
