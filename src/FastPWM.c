#include "FastPWM.h"

#include <avr/io.h>
#include <util/delay.h>
#include "Utils.h"
#include <can/can_wrapper.h>

static FastPWM instances[FAST_PWM_CONFIG_COUNT_MAX];
static uint8_t instancesCount = 0;

void FastPWM_Add(Pin* pin, uint16_t canId) {
   uint8_t index = instancesCount;
   
   Pin_SetMode(pin, PIN_OUTPUT);
   Pin_EnableAnalog(pin, PIN_7KHz);
   
   instances[index].canId = canId;
   instances[index].pin = pin;
   
   instancesCount++;
}

bool FastPWM_OnMessage(can_t* canMsg) {
	size_t i;
	
	for (i = 0; i < instancesCount; i++) {
		if (unlikely(canMsg->id == instances[i].canId)) {
			Pin_WriteAnalog(instances[i].pin, canMsg->data[0]);
			return true;
		}
	}
	
	return false;
}
