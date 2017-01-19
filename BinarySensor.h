#include <stdint.h>
#include "can.h"
#include "can_wrapper.h"
#include "Utils.h"

typedef struct _BinarySensor {
    volatile uint8_t* port;
    uint8_t pin;
    uint16_t canId;
    uint8_t state;
} BinarySensor;

extern void BinarySensorInit(
        BinarySensor* binarySensor,
        volatile uint8_t* ddr,
        volatile uint8_t* port,
        uint8_t pin,
        uint16_t canId
);

extern void BinarySensorProbe(BinarySensor* binarySensor);