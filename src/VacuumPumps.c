#include "VacuumPumps.h"

static uint16_t timer = 0;
static VacuumPump instances[VACUUM_PUMP_CONFIG_COUNT_MAX];
static SVacuumPump s_instances[VACUUM_PUMP_CONFIG_COUNT_MAX];
static uint8_t count = 0;
//
// ISR(TIMER0_COMP){
//
//   if(timer >= 0) {
//     timer --;
//   }
//
// }

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

void SpecialPump_Add(Pin* pumpPin, Pin* switchPin1,Pin* switchPin2, uint8_t number) {

	// Set mode
	Pin_SetMode(pumpPin, PIN_OUTPUT);
	Pin_SetMode(switchPin1, PIN_OUTPUT);
	Pin_SetMode(switchPin2, PIN_OUTPUT);

	Pin_WriteDigital(pumpPin, PIN_LOW);
	Pin_WriteDigital(switchPin1, PIN_LOW);
	Pin_WriteDigital(switchPin2, PIN_LOW);

  // Initialize values
	s_instances[count].canId = VACUUM_PUMP_CANID + number;
	s_instances[count].pumpPin = pumpPin;
	s_instances[count].switchPin1 = switchPin1;
	s_instances[count].switchPin2 = switchPin2;

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
					Pin_WriteDigital(instances[i].switchPin, PIN_LOW);
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

bool SingleSpecialPump_OnMessage(can_t* canMsg) {
	size_t i;
	for(i = 0; i < count; i++) {
		if(unlikely(canMsg->id == s_instances[i].canId)) {
		//uint8_t num = s_instances[i].canId - VACUUM_PUMP_CANID;
				switch(canMsg->data[0]) {  // U ZAVISNOSTI OD FORMATA PORUKE PROMENITI
					case 0:
						Pin_WriteDigital(s_instances[i].pumpPin, PIN_LOW);
						Pin_WriteDigital(s_instances[i].switchPin1, PIN_LOW);
						Pin_WriteDigital(s_instances[i].switchPin2, PIN_LOW);
						break;
						/*Pumpa iskljucena / oba ventila u neaktiviranom polozaju*/
					case 1:
						Pin_WriteDigital(s_instances[i].pumpPin, PIN_HIGH);
						Pin_WriteDigital(s_instances[i].switchPin1, PIN_LOW);
						Pin_WriteDigital(s_instances[i].switchPin2, PIN_LOW);
						break;
						/* Pumpa ukljucena / hvata jedna sisaljka (u zavisnosti od povezivanja switchPin -ova)*/
					case 2:
						Pin_WriteDigital(s_instances[i].pumpPin, PIN_HIGH);
						Pin_WriteDigital(s_instances[i].switchPin1, PIN_LOW);
						Pin_WriteDigital(s_instances[i].switchPin2, PIN_HIGH);
						break;
						/*Pumpa ukljucena / hvata druga sisaljka (u zavisnosti od povezivanja switchPin -ova)*/
					case 3:
						Pin_WriteDigital(s_instances[i].pumpPin, PIN_HIGH);
						Pin_WriteDigital(s_instances[i].switchPin2, PIN_LOW);				
						Pin_WriteDigital(s_instances[i].switchPin1, PIN_HIGH);
						_delay_ms(VACUUM_SWITCH_RELASE_TIME);
						Pin_WriteDigital(s_instances[i].switchPin1, PIN_LOW);
						break;
						/* Pumpa ukljucena / otpusta prva sisaljka (spajanjem sisaljke preko prvog ventila sa atmosferom)*/
						/* nakon relase vremena, prvi ventil punovo spaja ventil sa sisaljkama na pumpu */
					case 4:
						Pin_WriteDigital(s_instances[i].pumpPin, PIN_HIGH);
						Pin_WriteDigital(s_instances[i].switchPin2, PIN_HIGH);
						Pin_WriteDigital(s_instances[i].switchPin1, PIN_HIGH);
						_delay_ms(VACUUM_SWITCH_RELASE_TIME);
						Pin_WriteDigital(s_instances[i].switchPin1, PIN_LOW);
						break;
						/* Pumpa ukljucena / otpusta druga sisaljka (spajanjem sisaljke preko prvog ventila sa atmosferom)*/
						/* nakon relase vremena, prvi ventil punovo spaja ventil sa sisaljkama na pumpu */	
				}
		}
	}
}

