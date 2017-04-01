#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include <can/can.h>
#include <can/can_wrapper.h>
#include "BinarySensor.h"
#include "AX12.h"
#include "Debugger.h"
#include "Utils.h"
#include "MCP.h"
#include "FastPWM.h"
#include "RX24.h"

int main(void) {
	Utils_Init();
	Debugger_Init();
	
	can_wrapper_init();
	AX12_InitAll(2000);
	RX24_InitAll(2001);
	
	can_wrapper_send(7000, 1, 1);
	
	MCP mcpFirstRow;
	MCP_Init(&mcpFirstRow, 0x07, &PORTB, &DDRB, PB0);
	
	MCP mcpSecondRow;
	MCP_Init(&mcpSecondRow, 0x00, &PORTB, &DDRB, PB0);

	// Enable interrupts
    sei();
    

    // Binary actuators
    (void)MCP_AddActuator(&mcpFirstRow, GPIOA, 0, 1000);
	(void)MCP_AddActuator(&mcpFirstRow, GPIOA, 1, 1001);
	(void)MCP_AddActuator(&mcpFirstRow, GPIOA, 2, 1002);
	(void)MCP_AddActuator(&mcpFirstRow, GPIOA, 3, 1003);
	(void)MCP_AddActuator(&mcpFirstRow, GPIOA, 4, 1004);
	(void)MCP_AddActuator(&mcpFirstRow, GPIOA, 5, 1005);
	(void)MCP_AddActuator(&mcpFirstRow, GPIOA, 6, 1006);
	
	(void)MCP_AddActuator(&mcpSecondRow, GPIOB, 4, 1014);
	(void)MCP_AddActuator(&mcpSecondRow, GPIOB, 5, 1015);
	(void)MCP_AddActuator(&mcpSecondRow, GPIOB, 6, 1016);
    
	(void)BinarySensor_Add(&DDRA, &PORTA, &PINA, PA2, 0x8008);
    
	// Binary sensors
	(void)BinarySensor_Add(&DDRC, &PORTC, &PINC, PC6, 0x8000);
	(void)BinarySensor_Add(&DDRC, &PORTC, &PINC, PC7, 0x8001); // Doesn't work, check the board
    (void)BinarySensor_Add(&DDRG, &PORTG, &PING, PG2, 0x8002);
    (void)BinarySensor_Add(&DDRA, &PORTA, &PINA, PA7, 0x8003);
    (void)BinarySensor_Add(&DDRA, &PORTA, &PINA, PA6, 0x8004);
    (void)BinarySensor_Add(&DDRA, &PORTA, &PINA, PA5, 0x8005);
    (void)BinarySensor_Add(&DDRA, &PORTA, &PINA, PA4, 0x8006);
    (void)BinarySensor_Add(&DDRA, &PORTA, &PINA, PA3, 0x8007);
    

	// PWMs
	(void)FastPWM_Add(&TCCR0A, &OCR0A, &DDRB, PB7, 3000);
	

    while (1) {
		// AX12_UpdateAll();
		BinarySensor_UpdateAll();

        // Check if a new message was received
        if (can_check_message()) {
            can_t msg;

            // Try to read the message
            if (can_get_message(&msg)) {
				MCP_OnMessage(&mcpFirstRow, &msg);
				MCP_OnMessage(&mcpSecondRow, &msg);
				AX12_OnMessage(&msg);
				RX24_OnMessage(&msg);
				FastPWM_OnMessage(&msg);
            }
        }
    }
	
	return 0;
}
