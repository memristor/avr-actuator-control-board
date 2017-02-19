#include "BinarySensor.h"

static BinarySensor instances[BINARY_SENSOR_CONFIG_MAX];
static uint8_t instancesCount = 0;

uint8_t BinarySensor_Add(
        volatile uint8_t* ddr,
        volatile uint8_t* port,
        volatile uint8_t* pin,
        uint8_t p,
        uint16_t canId
) {
	uint8_t index = instancesCount;
	
    // Set pull-up resistor
    set_bit(*port, p);

    // Set input pin
    clear_bit(*ddr, p);

    // Initialize values
    instances[index].canId = canId;
    instances[index].pin = pin;
    instances[index].p = p;
    instances[index].state = 0;
    
    // Return index
    instancesCount++;
    return index;
}

inline void BinarySensor_UpdateAll(void) {
	size_t i;
	for (i = 0; i < instancesCount; i++) {
		if (bit_is_set(*(instances[i].pin), instances[i].p) &&
				instances[i].state == 0) {
			instances[i].state = 1;
			can_wrapper_send(instances[i].canId, 1, instances[i].state);
		}
		else if (bit_is_clear(*(instances[i].pin), instances[i].p) &&
				instances[i].state == 1) {
			instances[i].state = 0;
			can_wrapper_send(instances[i].canId, 1, instances[i].state);
		}
	}
}
