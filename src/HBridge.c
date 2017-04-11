#include "HBridge.h"

#include "Utils.h"
#include <can/can_wrapper.h>

static HBridge instances[5];
static uint8_t count = 0;

void HBridge_Add(Pin* inA, Pin* inB, Pin* inH, uint16_t canId) {
	instances[count].inA = inA;
	instances[count].inB = inB;
	instances[count].inH = inH;
	instances[count].canId = canId;

	set_bit(DDRB, PB5);
	set_bit(DDRC, PC3);
	set_bit(DDRC, PC4);

	TCCR1A |= (1 << WGM10) | (1 << WGM11) | (1 << COM1A1) | (1 << CS10);
	clear_bit(TCCR1A, CS11);
	clear_bit(TCCR1A, CS12);
	
	set_bit(DDRC, PC3);
	set_bit(DDRC, PC4);
	set_bit(PORTC, PC3);
	clear_bit(PORTC, PC4);
	
	
	OCR1A = 500;

	count++;
}

void HBridge_OnMessage(can_t* canMsg) {
	size_t i;
	
	for (i = 0; i < count; i++) {
		if (unlikely(canMsg->id == instances[i].canId)) {
			
			// Set direction
			if (canMsg->data[1] == 0) { 
				// ...
			} else {
				// ...
			}
			
			// Set output
			if (canMsg->data[0] == 0) {
				// Write 0
			} else {
				// Write value
			}
		}
	}
}
