#include "BinaryActuator.h"

inline void BinaryActuatorInit(
        BinaryActuator* binaryActuator,
        volatile uint8_t* ddr,
        volatile uint8_t* port,
        uint8_t pin,
        uint16_t canId
) {
    // Set pin as an output pin
    SET_BIT(*ddr, pin);

    binaryActuator->port = port;
    binaryActuator->pin = pin;
    binaryActuator->canId = canId;
}

inline void BinaryActuatorProbe(BinaryActuator* binaryActuator, can_t* canMsg) {
    if (unlikely(canMsg->id == binaryActuator->canId)) {
        if (canMsg->data[0] == 0x01) {
            SET_BIT(*(binaryActuator->port), binaryActuator->pin);
        } else {
            CLEAR_BIT(*(binaryActuator->port), binaryActuator->pin);
        }
    }
}