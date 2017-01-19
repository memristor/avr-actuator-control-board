#include <stdint.h>
#include "can.h"
#include "Utils.h"

enum BinaryActuatorError {
    BINARY_ACTUATOR_SUCCESS = 0,
    BINARY_ACTUATOR_ERROR
};

typedef struct _BinaryActuator {
    volatile uint8_t* port;
    uint8_t pin;
    uint16_t canId;
} BinaryActuator;


// Reference: http://www.atmel.com/webdoc/AVRLibcReferenceManual/FAQ_1faq_port_pass.html
extern void BinaryActuatorInit(
        BinaryActuator* binaryActuator,
        volatile uint8_t* ddr,
        volatile uint8_t* port,
        uint8_t pin,
        uint16_t canId
);

extern void BinaryActuatorProbe(BinaryActuator* binaryActuator, can_t* canMsg);