#include "BinarySensor.h"

inline void BinarySensorInit(
        BinarySensor* binarySensor,
        volatile uint8_t* ddr,
        volatile uint8_t* port,
        volatile uint8_t* pin,
        uint8_t p,
        uint16_t canId
) {
    // Set pull-up resistor
    set_bit(*port, p);

    // Set input pin
    clear_bit(*ddr, p);

    // Initialize values
    binarySensor->canId = canId;
    binarySensor->pin = pin;
    binarySensor->p = p;
    binarySensor->state = 0;
}

inline void BinarySensorProbe(BinarySensor* binarySensor) {
    if (bit_is_set(*binarySensor->pin, binarySensor->p) &&
            binarySensor->state == 0) {
        binarySensor->state = 1;
        can_wrapper_send(binarySensor->canId, 1, binarySensor->state);
    }
    else if (bit_is_clear(*binarySensor->pin, binarySensor->p) &&
            binarySensor->state == 1) {
        binarySensor->state = 0;
        can_wrapper_send(binarySensor->canId, 1, binarySensor->state);
    }
}
