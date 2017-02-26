#include "FastPWM.h"

#include <avr/io.h>
#include <util/delay.h>
#include "Utils.h"
#include <can/can_wrapper.h>

static FastPWM instances[FAST_PWM_CONFIG_COUNT_MAX];
static uint8_t instancesCount = 0;

uint8_t FastPWM_Add(volatile uint8_t* counter, volatile uint8_t* ocr, volatile uint8_t* ddr, uint8_t p, uint16_t canId) {
   uint8_t index = instancesCount;
   
   (*counter) |= (1<<WGM00) | (1<<WGM01) | (1<<COM0A1) | (1<<CS00);
   (*ddr) |= (1 << p);
   
   instances[index].canId = canId;
   instances[index].ocr = ocr;
   
   instancesCount++;
   
   return index;
}

void FastPWM_OnMessage(can_t* canMsg) {
	size_t i;
	
	for (i = 0; i < instancesCount; i++) {
		if (unlikely(canMsg->id == instances[i].canId)) {
			*(instances[i].ocr) = canMsg->data[0];
		}
	}
}
