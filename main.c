#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <util/delay.h>
#include "can.h"
#include "can_wrapper.h"
#include "BinaryActuator.h"
#include "BinarySensor.h"

// Define number of actuators and sensors
#define BINARY_ACTUATORS_NUMBER 1
#define BINARY_SENSORS_NUMBER   2

BinaryActuator binaryActuators[BINARY_ACTUATORS_NUMBER];
BinarySensor binarySensors[BINARY_SENSORS_NUMBER];

size_t i;
int main(void) {
    can_wrapper_init();


    // Insert actuators and sensors here!
    // Do not forget to update a number of actuators and sensors
    BinaryActuatorInit(&binaryActuators[0], &DDRB, &PORTB, PB0, 0x01);
    BinarySensorInit(&binarySensors[0], &DDRG, &PORTG, PG2, 0x02);
    BinarySensorInit(&binarySensors[1], &DDRA, &PORTA, PA7, 0x03);


    while (1) {
        for (i = 0; i < BINARY_SENSORS_NUMBER; i++) {
            BinarySensorProbe(&binarySensors[i]);
        }

        // Check if a new message was received
        if (can_check_message()) {
            can_t msg;

            // Try to read the message
            if (can_get_message(&msg)) {
                for (i = 0; i < BINARY_ACTUATORS_NUMBER; i++) {
                    BinaryActuatorProbe(&binaryActuators[i], &msg);
                }
            }
        }
    }
	
	return 0;
}