#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <util/delay.h>
#include "can.h"
#include "can_wrapper.h"

uint8_t lastStateG = 0;
uint8_t stateG = 0;
uint8_t lastStateA = 0;
uint8_t stateA = 0;

int main(void) {
    can_wrapper_init();

    DDRG = 0x00;
    DDRA = 0x00;

    while (1) {
        stateG = (PING & (1 << PG2)) >> PG2;
        if (stateG != lastStateG) {
            lastStateG = stateG;
            can_wrapper_send(0x08, 1, stateG);
        }

        stateA = (PINA & (1 << PA7)) >> PA7;
        if (stateA != lastStateA) {
            lastStateA = stateA;
            can_wrapper_send(0x09, 1, stateA);
        }

        // Check if a new messag was received
        if (can_check_message()) {
            can_t msg;

            // Try to read the message
            if (can_get_message(&msg)) {
                msg.data[0]++;
                can_send_message(&msg);
            }
        }
    }
	
	return 0;
}