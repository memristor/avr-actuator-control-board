#include "BinarySensor.h"

inline void BinarySensorInit(
        BinarySensor* binarySensor,
        volatile uint8_t* ddr,
        volatile uint8_t* port,
        uint8_t pin,
        uint16_t canId
) {
    // Set pull-up resistor
    SET_BIT(*port, pin);

    // Set input pin
    CLEAR_BIT(*ddr, pin);

    // Initialize values
    binarySensor->canId = canId;
    binarySensor->port = port;
    binarySensor->pin = pin;
    binarySensor->state = 0;
}

inline void BinarySensorProbe(BinarySensor* binarySensor) {
    if (bit_is_set(*binarySensor->port, binarySensor->pin) &&
            binarySensor->state == 0) {
        binarySensor->state = 1;
        can_wrapper_send(binarySensor->canId, 1, binarySensor->state);
    }
    else if (bit_is_clear(*binarySensor->port, binarySensor->pin) &&
            binarySensor->state == 1) {
        binarySensor->state = 0;
        can_wrapper_send(binarySensor->canId, 1, binarySensor->state);
    }
}