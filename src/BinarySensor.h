#ifndef _BINARY_SENSOR_H_
#define _BINARY_SENSOR_H_

#include <stdint.h>
#include <can/can.h>
#include <can/can_wrapper.h>
#include "Utils.h"

typedef struct _BinarySensor {
    volatile uint8_t* pin;
    uint8_t p;
    uint16_t canId;
    uint8_t state;
} BinarySensor;

extern void BinarySensorInit(
        BinarySensor* binarySensor,
        volatile uint8_t* ddr,
        volatile uint8_t* port,
        volatile uint8_t* pin,
        uint8_t p,
        uint16_t canId
);

extern void BinarySensorProbe(BinarySensor* binarySensor);

#endif
