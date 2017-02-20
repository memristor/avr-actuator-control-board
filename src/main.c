#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include <can/can.h>
#include <can/can_wrapper.h>
#include "BinaryActuator.h"
#include "BinarySensor.h"
#include "AX12.h"
#include "Debugger.h"

#include <dynamixel/ax.h>
#include <dynamixel/dynamixel.h>


int main(void) {
	Debugger_Init();
	AX12_InitAll(2000);
	BinaryActuator_InitAll((uint8_t []) { 0x07, 0x00 }, 2);
    can_wrapper_init();
    
	// Enable interrupts
    sei();
    
    
    if (false) {
		bool state = true;
		_delay_ms(2000);
		dynamixel_writebyte(DYNAMIXEL_BROADCAST_ID, AX_LED, 1);
		_delay_ms(500);
		dynamixel_writeword(DYNAMIXEL_BROADCAST_ID, AX_GOAL_SPEED_L, 200);
		_delay_ms(500);
		dynamixel_writeword(DYNAMIXEL_BROADCAST_ID, AX_GOAL_POSITION_L, 200);
	}

	

    // Binary actuators
    (void)BinaryActuator_Add(0x07, GPIOA, 0, 0x10);
    (void)BinaryActuator_Add(0x07, GPIOA, 1, 0x11);

	// Binary sensors
	(void)BinarySensor_Add(&DDRC, &PORTC, &PINC, PC6, 0x8000);
	(void)BinarySensor_Add(&DDRC, &PORTC, &PINC, PC7, 0x8001); // Doesn't work, check the board
    (void)BinarySensor_Add(&DDRG, &PORTG, &PING, PG2, 0x8002);
    (void)BinarySensor_Add(&DDRA, &PORTA, &PINA, PA7, 0x8003);
    (void)BinarySensor_Add(&DDRA, &PORTA, &PINA, PA6, 0x8004);
    (void)BinarySensor_Add(&DDRA, &PORTA, &PINA, PA5, 0x8005);
    (void)BinarySensor_Add(&DDRA, &PORTA, &PINA, PA4, 0x8006);
    (void)BinarySensor_Add(&DDRA, &PORTA, &PINA, PA3, 0x8007);

    while (1) {
		BinarySensor_UpdateAll();

        // Check if a new message was received
        if (can_check_message()) {
            can_t msg;

            // Try to read the message
            if (can_get_message(&msg)) {
				BinaryActuator_OnMessage(&msg);
				AX12_OnMessage(&msg);
            }
        }
    }
	
	return 0;
}
