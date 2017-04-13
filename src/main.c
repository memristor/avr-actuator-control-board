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
#include "Utils.h"
#include "HBridge.h"
#include "Pin.h"

int main(void) {
	Utils_Init();
	Debugger_Init();
	
	can_wrapper_init();
	AX12_InitAll(2000);
	RX24_InitAll(2001);
	
	can_wrapper_send(7000, 1, 1);
	
	MCP mcpFirstRow;
	MCP_Init(&mcpFirstRow, 0x07, &PORTB, &DDRB, PB0);
    
    // Binary actuators
    MCP_AddActuator(&mcpFirstRow, GPIOA, 0, 1000);
	MCP_AddActuator(&mcpFirstRow, GPIOA, 1, 1001);
	MCP_AddActuator(&mcpFirstRow, GPIOA, 2, 1002);
	MCP_AddActuator(&mcpFirstRow, GPIOA, 3, 1003);
	MCP_AddActuator(&mcpFirstRow, GPIOA, 4, 1004);
	MCP_AddActuator(&mcpFirstRow, GPIOA, 5, 1005);
	MCP_AddActuator(&mcpFirstRow, GPIOA, 6, 1006);
    
	// Infrareds
	BinarySensor_Add(&Pin_C6, 64000);
	BinarySensor_Add(&Pin_C7, 64001); // Doesn't work, check the board
	BinarySensor_Add(&Pin_G2, 64002);
	BinarySensor_Add(&Pin_A7, 64003);
	BinarySensor_Add(&Pin_A6, 64004);
	BinarySensor_Add(&Pin_A5, 64005);
	BinarySensor_Add(&Pin_A4, 64006);
	BinarySensor_Add(&Pin_A3, 64007);
	BinarySensor_Add(&Pin_A2, 64008);

	// PWMs
	// FastPWM_Add(&Timer0, &PinB7, 3000);
	
	// H-Bridges
	HBridge_Add(&Pin_C3, &Pin_C4, &Pin_B5, 1101);
	
	
	// Enable interrupts
    sei();

    while (1) {
		// AX12_UpdateAll();
		BinarySensor_UpdateAll();

        // Check if a new message was received
        if (can_check_message()) {
            can_t msg;

            // Try to read the message
            if (can_get_message(&msg)) {
				MCP_OnMessage(&mcpFirstRow, &msg);
				AX12_OnMessage(&msg);
				RX24_OnMessage(&msg);
				FastPWM_OnMessage(&msg);
				HBridge_OnMessage(&msg);
            }
        }
    }
	
	return 0;
}
