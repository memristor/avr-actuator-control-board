#include "BinarySensor.h"

static BinarySensor instances[BINARY_SENSOR_CONFIG_MAX];
static uint8_t instancesCount = 0;

void BinarySensor_Add(Pin* pin, uint16_t canId) {
	uint8_t index = instancesCount;
	
	// Set mode
    Pin_SetMode(pin, PIN_INPUT);

    // Initialize values
    instances[index].canId = canId;
    instances[index].pin = pin;
    instances[index].state = 0;
    
    // Return index
    instancesCount++;
}

inline void BinarySensor_UpdateAll(void) {
	size_t i;
	for (i = 0; i < instancesCount; i++) {
		if (Pin_ReadDigital(instances[i].pin) == PIN_HIGH && instances[i].state == 0) {
			instances[i].state = 1;
			can_wrapper_send(instances[i].canId, 1, instances[i].state);
		}
		else if (Pin_ReadDigital(instances[i].pin) == PIN_LOW && instances[i].state == 1) {
			instances[i].state = 0;
			can_wrapper_send(instances[i].canId, 1, instances[i].state);
		}
	}
}
