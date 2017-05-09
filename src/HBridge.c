#include "HBridge.h"

#include "Utils.h"
#include <can/can_wrapper.h>

static HBridge instances[HBRIDGE_CONFIG_COUNT_MAX];
static uint8_t count = 0;

void HBridge_Add(Pin* inA, Pin* inB, Pin* inH, PinFrequency frequency, uint16_t canId) {
	instances[count].inA = inA;
	instances[count].inB = inB;
	instances[count].inH = inH;
	instances[count].canId = canId;

	Pin_SetMode(inH, PIN_OUTPUT);
	Pin_SetMode(inA, PIN_OUTPUT);
	Pin_SetMode(inB, PIN_OUTPUT);
    Pin_EnableAnalog(inH, frequency);

	count++;
}

bool HBridge_OnMessage(can_t* canMsg) {
	size_t i;
	
	for (i = 0; i < count; i++) {
		if (unlikely(canMsg->id == instances[i].canId)) {
			
			switch(canMsg->data[1]) {
				case 0: // Left
					Pin_WriteDigital(instances[i].inA, PIN_HIGH);
					Pin_WriteDigital(instances[i].inB, PIN_LOW);
					break;
				case 1:	// Right
					Pin_WriteDigital(instances[i].inA, PIN_LOW);
					Pin_WriteDigital(instances[i].inB, PIN_HIGH);
					break;
				case 2:	// Stop
					Pin_WriteDigital(instances[i].inA, PIN_HIGH);
					Pin_WriteDigital(instances[i].inB, PIN_HIGH);
					break;
			}
			
			// Set output
			Pin_WriteAnalog(instances[i].inH, canMsg->data[0]);
			
			return true;
		}
	}
	
	return false;
}
