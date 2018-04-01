#include "VacuumPumps.h"


static VacuumPump instances[VACUUM_PUMP_CONFIG_COUNT_MAX];
static uint8_t count = 0;

void VacuumPump_Add(Pin* pumpPin, Pin* switchPin, uint8_t number) {

	// Set mode
  Pin_SetMode(pumpPin, PIN_OUTPUT);
	Pin_SetMode(switchPin, PIN_OUTPUT);

	Pin_WriteDigital(pumpPin, PIN_LOW);
	Pin_WriteDigital(switchPin, PIN_LOW);

  // Initialize values
  instances[count].canId = VACUUM_PUMP_CANID + number;
  instances[count].pumpPin = pumpPin;
	instances[count].switchPin = switchPin;

  // Return index
  count++;

}

bool VacuumPump_OnMessage(can_t* canMsg) {
	size_t i;
	bool tmpCheck = false;

	if(unlikely(canMsg->id == VACUUM_PUMP_CANID)) {
		for(i = 0; i < count; i++) {

			switch(canMsg->data[i]) {
				case 0:
					Pin_WriteDigital(instances[i].pumpPin, PIN_LOW);
					Pin_WriteDigital(instances[i].switchPin, PIN_HIGH);
					tmpCheck = true;
					break;

				case 1:
					Pin_WriteDigital(instances[i].pumpPin, PIN_HIGH);
					break;

			}


		}
		if(tmpCheck == true) {
			_delay_ms(VACUUM_SWITCH_RELASE_TIME);

			for(i = 0; i < count; i++) {
				Pin_WriteDigital(instances[i].switchPin, PIN_LOW);
			}
		}

		return true;
	}

	return false;

}

bool SingleVacuumPump_OnMessage(can_t* canMsg) {
	size_t i;
	bool tmpCheck = false;

	for(i = 0; i < count; i++) {
		if(unlikely(canMsg->id == instances[i].canId)) {

			switch(canMsg->data[0]) {
				case 0:
					Pin_WriteDigital(instances[i].pumpPin, PIN_LOW);
					Pin_WriteDigital(instances[i].switchPin, PIN_HIGH);
					tmpCheck = true;
					break;

				case 1:
					Pin_WriteDigital(instances[i].pumpPin, PIN_HIGH);
					Pin_WriteDigital(instances[i].switchPin, PIN_LOW);
					break;
			}

			if(tmpCheck == true) {
				_delay_ms(VACUUM_SWITCH_RELASE_TIME);
				Pin_WriteDigital(instances[i].switchPin, PIN_LOW);
			}

			return true;
		}
	}

	return false;
}
