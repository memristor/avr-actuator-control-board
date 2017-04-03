#ifndef _HBRIDGE_H_
#define _HBRIDGE_H_

#include <inttypes.h>
#include <can/can.h>

typedef struct _HBridge {
	uint8_t ina;
	uint8_t inb;
	uint8_t inh;
	uint16_t canId;
	volatile uint8_t* port;
} HBridge;

/**
 *	@example HBridge_Add(&DDRC, &PORTC, PC0, PC1, PC2, 1101);
 */
void HBridge_Add(
	volatile uint8_t* ddr,
	volatile uint8_t* port,
	uint8_t ina,
	uint8_t inb,
	uint8_t inh,
	uint16_t canId
);

void HBridge_OnMessage(can_t* canMsg);

#endif
