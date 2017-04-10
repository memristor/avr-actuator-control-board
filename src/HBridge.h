#ifndef _HBRIDGE_H_
#define _HBRIDGE_H_

#include <inttypes.h>
#include <can/can.h>
#include "Utils.h"
#include "Pin.h"

typedef struct _HBridge {
	Pin* inA;
	Pin* inB;
	Pin* inH;
	uint16_t canId;
} HBridge;

/**
 * @example HBridge_Add(&PinC0, &PinC1, &PinC2, 1101);
 */
void HBridge_Add(Pin* inA, Pin* inB, Pin* inH, uint16_t canId);

void HBridge_OnMessage(can_t* canMsg);

#endif
