#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"

#include "RegisterFile.h"
#include "SystemDelay.h"
#include "MODBUS.h"
#include "ADPS9960.h"
#include "timers.h"
#include "IODrivers.h"

void Check_distance(void) {
	uint16_t distance = 0;
	uint16_t threshold_max;
	uint16_t threshold_min;

	distance = ADPS9960_GetDistance();
	threshold_max = register_direct_read (REGISTER_THRES_HIGH);
	threshold_min = register_direct_read (REGISTER_THRES_LOW);

	register_direct_write(REGISTER_DISTANCE, distance);

	if ( (distance > threshold_max) || (distance < threshold_min) ) {
		RelayOn (0);
		LEDOn();
	} else {
		RelayOff (0);
		LEDOff();
	}
}

int main(void) {
	/* Chip errata */
	CHIP_Init();

	CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

	Drivers_init();
	register_init();
	SysTicksInit();
	modbus_init();

	ADPS9960_Init();

	Timers_init();

	Timers_setDuty(3);

	/* Infinite loop */
	while (1) {
		SysTicksDisable();
		EMU_EnterEM2(true);
		SysTicksEnable();

		modbus_slave();

		if (Timers_triggered()) {
			Timers_processed();
			Check_distance();
		}
	}
}
