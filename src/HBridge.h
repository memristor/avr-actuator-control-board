#ifndef _HBRIDGE_H_
#define _HBRIDGE_H_

#include <inttypes.h>
#include <can/can.h>
#include <stdbool.h>
#include "Utils.h"
#include "Pin.h"
#include "Config.h"

#ifndef HBRIDGE_CONFIG_COUNT_MAX
#define HBRIDGE_CONFIG_COUNT_MAX 10
#warning "Max number of HBridge update is not redefined"
#endif

typedef struct _HBridge {
	Pin* inA;
	Pin* inB;
	Pin* inH;
	uint16_t canId;
} HBridge;

/**
 * @example HBridge_Add(&PinC0, &PinC1, &PinC2, 1101);
 */
void HBridge_Add(Pin* inA, Pin* inB, Pin* inH, PinFrequency frequency, uint16_t canId);

bool HBridge_OnMessage(can_t* canMsg);

#endif
