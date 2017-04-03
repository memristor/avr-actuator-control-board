#include "HBridge.h"

#include "Utils.h"
#include <can/can_wrapper.h>

static HBridge instances[5];
static uint8_t count = 0;

void HBridge_Add(
	volatile uint8_t* ddr,
	volatile uint8_t* port,
	uint8_t ina,
	uint8_t inb,
	uint8_t inh,
	uint16_t canId
) {
	set_bit(*ddr, ina); // Set INA as output
	set_bit(*ddr, inb);	// Set INB as output
	set_bit(*ddr, inh);	// Set INH as output
	
	instances[count].ina = ina;
	instances[count].inb = inb;
	instances[count].inh = inh;
	instances[count].port = port;
	instances[count].canId = canId;
	
	count++;
}

void HBridge_OnMessage(can_t* canMsg) {
	size_t i;
	
	for (i = 0; i < count; i++) {
		if (unlikely(canMsg->id == instances[i].canId)) {
			
			// Set direction
			if (canMsg->data[1] == 0) { 
				clear_bit(*(instances[i].port), instances[i].ina);
				set_bit(*(instances[i].port), instances[i].inb);
			} else {
				clear_bit(*(instances[i].port), instances[i].inb);
				set_bit(*(instances[i].port), instances[i].ina);
			}
			
			// Set output
			if (canMsg->data[0] == 0) {
				clear_bit(*(instances[i].port), instances[i].inh);
			} else {
				set_bit(*(instances[i].port), instances[i].inh);
			}
		}
	}
}
