#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <util/delay.h>

#include "can.h"


// -----------------------------------------------------------------------------
// Main loop for receiving and sending messages.

int main(void) {
	// Initialize MCP2515
	can_init(BITRATE_125_KBPS);

    // Set filter to receive all messages
    can_filter_t filter = {
        .id = 0,
        .mask = 0,
        .flags = {
            .rtr = 0,
            .extended = 0
        }
    };
    can_set_filter(0, &filter);

    // Enable interrupts
    sei();
	
	// Create a test message
	can_t msg;
	
	msg.id = 0x123456;
	msg.flags.rtr = 0;
	msg.flags.extended = 1;
	
	msg.length = 4;
	msg.data[0] = 0xce;
	msg.data[1] = 0xad;
	msg.data[2] = 0xbe;
	msg.data[3] = 0xef;

    can_send_message({});

    while (1)
    {
        // Check if a new messag was received
        if (can_check_message())
        {
            can_t msg;

            // Try to read the message
            if (can_get_message(&msg))
            {
                // If we received a message resend it with a different id
                msg.id += 10;

                // Send the new message
                can_send_message(&msg);
            }
        }


    }
	
	return 0;
}
