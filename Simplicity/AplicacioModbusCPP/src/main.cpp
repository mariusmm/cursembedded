#include "em_device.h"
#include "em_chip.h"
#include "em_emu.h"

#include <array>
#include <atomic>
#include <functional>

#include "Register.hpp"
#include "SystemDelay.hpp"
#include "IOPin.hpp"
#include "ADPS9960.hpp"
#include "UART.hpp"
#include "Modbus.hpp"
#include "Timer.hpp"

const uint8_t DEVICE_ADDR { 0x39 << 1 };

class Application : Register_cb {
public:
	Application() : my_sensor(), mod_client(&leuart, &register_file) {

		letimer.SetDuty(3);

		register_file.register_wr_callback(REGISTER_DUTYCYLE, this);
		register_file.register_wr_callback(REGISTER_DOREAD, this);
		register_file.register_wr_callback(REGISTER_LED, this);
		register_file.register_wr_callback(REGISTER_RELAY1, this);
		register_file.register_wr_callback(REGISTER_RELAY2, this);
	}

	void process(void) {

		mod_client.Slave();

		if (letimer.Triggered()) {
			letimer.Processed();
			uint16_t distance = 0;
			uint16_t threshold_max;
			uint16_t threshold_min;

			distance = my_sensor.GetDistance();
			threshold_max = register_file.direct_read(REGISTER_THRES_HIGH);
			threshold_min = register_file.direct_read(REGISTER_THRES_LOW);

			register_file.direct_write(REGISTER_DISTANCE, distance);

			if ((distance > threshold_max) || (distance < threshold_min)) {
				RELAY1Pin.Set();
				LEDPin.Set();
			} else {
				RELAY1Pin.Reset();
				LEDPin.Reset();
			}
		}
	}

	void callback(unsigned int reg_num, int data) {
		switch (reg_num) {
		case REGISTER_LED:
			app_LEDUpdate_cb (reg_num, data);
			break;
		case REGISTER_RELAY1:
		case REGISTER_RELAY2:
			app_RelayUpdate_cb (reg_num, data);
			break;
		case REGISTER_DUTYCYLE:
			app_timer_reg_cb  (reg_num, data);
			break;
		case REGISTER_DOREAD:
			app_APDS9960Distance_cb (reg_num, data);
			break;
		default:
			break;
		}
	}

private:
	void app_LEDUpdate_cb(int reg, int data) {
		(void) reg;
		if (data == 0) {
			LEDPin.Reset();
		} else {
			LEDPin.Set();
		}
	}

	void app_RelayUpdate_cb(int reg, int data) {

		if (data == 0) {
			if (reg == REGISTER_RELAY1) {
				RELAY1Pin.Reset();
			} else {
				RELAY2Pin.Reset();
			}

		} else {
			if (reg == REGISTER_RELAY1) {
				RELAY1Pin.Set();
			} else {
				RELAY2Pin.Set();
			}
		}
	}

	void app_timer_reg_cb(int reg, int data) {
		(void) reg;
		Timers::SetDuty(data);
	}

	void app_APDS9960Distance_cb(int reg, int data) {
		uint8_t aux;
		(void) reg;
		(void) data;
		aux = my_sensor.GetDistance();
		register_file.direct_write (REGISTER_DISTANCE, aux);
	}

private:
	Timers letimer;
	Register register_file;
	PIN<IOPin::PORTC, IOPin::PIN12, IOPin::PIN_OUT> LEDPin;
	PIN<IOPin::PORTC, IOPin::PIN4, IOPin::PIN_OUT> RELAY1Pin;
	PIN<IOPin::PORTC, IOPin::PIN5, IOPin::PIN_OUT> RELAY2Pin;
	PIN<IOPin::PORTD, IOPin::PIN7, IOPin::PIN_I2C> SCLPin;
	PIN<IOPin::PORTD, IOPin::PIN6, IOPin::PIN_I2C> SDAPin;

	UART<LEUARTA, 0> leuart;
	ADPS9960<I2C_DEV0, LOC1, DEVICE_ADDR> my_sensor;
	Modbus mod_client;
};

int main(void) {
	/* Chip errata */
	CHIP_Init();

	CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
	CMU_ClockEnable(cmuClock_HFLE, true);

	SysTick_mng.Enable();

	Application app;

	while(1) {

		SysTick_mng.Disable();
		EMU_EnterEM2(true);
		SysTick_mng.Enable();

		app.process();
	}

}
