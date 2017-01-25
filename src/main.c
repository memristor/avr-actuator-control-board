#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <util/delay.h>
#include <can/can.h>
#include <can/can_wrapper.h>
#include "BinaryActuator.h"
#include "BinarySensor.h"

// Define number of actuators and sensors
#define BINARY_ACTUATORS_NUMBER 1
#define BINARY_SENSORS_NUMBER   8

BinaryActuator binaryActuators[BINARY_ACTUATORS_NUMBER];
BinarySensor binarySensors[BINARY_SENSORS_NUMBER];

size_t i;
int main(void) {
    can_wrapper_init();

	

    // Insert actuators and sensors here!
    // Do not forget to update a number of actuators and sensors
    BinaryActuatorInitAll();
    BinaryActuatorInit(&binaryActuators[0], &DDRB, &PINB, PB0, 101);

	BinarySensorInit(&binarySensors[0], &DDRC, &PINC, PC6, 0x8000);
	BinarySensorInit(&binarySensors[1], &DDRC, &PINC, PC7, 0x8001); // Doesn't work, check the board
    BinarySensorInit(&binarySensors[2], &DDRG, &PING, PG2, 0x8002);
    BinarySensorInit(&binarySensors[3], &DDRA, &PINA, PA7, 0x8003);
    BinarySensorInit(&binarySensors[4], &DDRA, &PINA, PA6, 0x8004);
    BinarySensorInit(&binarySensors[5], &DDRA, &PINA, PA5, 0x8005);
    BinarySensorInit(&binarySensors[6], &DDRA, &PINA, PA4, 0x8006);
    BinarySensorInit(&binarySensors[7], &DDRA, &PINA, PA3, 0x8007);
    


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
