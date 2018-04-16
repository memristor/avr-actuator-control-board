#include "HBridge.h"

#include "Utils.h"
#include <can/can_wrapper.h>

static HBridge instances[HBRIDGE_CONFIG_COUNT_MAX];
static uint8_t count = 0;

void HBridge_Add(Pin* inH, Pin* inA, Pin* inB, PinFrequency frequency, uint8_t number) {
	instances[count].inH = inH;
	instances[count].inA = inA;
	instances[count].inB = inB;
	instances[count].canId = H_BRIDGE_CANID + number;

	Pin_SetMode(inH, PIN_OUTPUT);
	Pin_SetMode(inA, PIN_OUTPUT);
	Pin_SetMode(inB, PIN_OUTPUT);
    Pin_EnableAnalog(inA, frequency);

	count++;
}

bool HBridge_OnMessage(can_t* canMsg) {
	size_t i;

	for (i = 0; i < count; i++) {
		if (unlikely(canMsg->id == instances[i].canId)) {

			switch(canMsg->data[1]) {
				case 0: // Left
					Pin_WriteAnalog(instances[i].inA, 80);
					Pin_WriteDigital(instances[i].inB, PIN_LOW);
					Pin_WriteDigital(instances[i].inH, PIN_HIGH);
					break;
				case 1:	// Right
					Pin_WriteAnalog(instances[i].inA, 20);
					Pin_WriteDigital(instances[i].inB, PIN_HIGH);
					Pin_WriteDigital(instances[i].inH, PIN_HIGH);
					break;
				case 2:	// Stop
					Pin_WriteAnalog(instances[i].inA, 0);
					Pin_WriteDigital(instances[i].inB, PIN_LOW);
					Pin_WriteDigital(instances[i].inH, PIN_LOW);
					break;
			}

		return true;
		}
	}

	return false;
}
