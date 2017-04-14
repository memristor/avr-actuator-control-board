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

MCP mcp;

int main(void) {
	Utils_Init();
	Debugger_Init();
	AX12_InitAll(0x00007F00);
	RX24_InitAll(0x00007F01);
	MCP_Init(&mcp, 0x07, &PORTB, &DDRB, PB0);
	
	// Binary actuators
    MCP_AddActuator(&mcp, GPIOA, 0, 0x00007F02);
	MCP_AddActuator(&mcp, GPIOA, 1, 0x00007F03);
	MCP_AddActuator(&mcp, GPIOA, 2, 0x00007F04);
	MCP_AddActuator(&mcp, GPIOA, 3, 0x00007F05);
	MCP_AddActuator(&mcp, GPIOA, 4, 0x00007F06);
	MCP_AddActuator(&mcp, GPIOA, 5, 0x00007F07);
	MCP_AddActuator(&mcp, GPIOA, 6, 0x00007F08);
    
	// Infrareds
	BinarySensor_Add(&Pin_C6, 0x00007F09);
	BinarySensor_Add(&Pin_C7, 0x00007F0A); // Doesn't work, check the board
	BinarySensor_Add(&Pin_G2, 0x00007F0B);
	BinarySensor_Add(&Pin_A7, 0x00007F0C);
	BinarySensor_Add(&Pin_A6, 0x00007F0D);
	BinarySensor_Add(&Pin_A5, 0x00007F0E);
	BinarySensor_Add(&Pin_A4, 0x00007F0F);
	BinarySensor_Add(&Pin_A3, 0x00007F10);
	BinarySensor_Add(&Pin_A2, 0x00007F11);

	// PWMs
	// FastPWM_Add(&Timer0, &PinB7, 0x00007F12);
	
	// H-Bridges
	HBridge_Add(&Pin_C3, &Pin_C4, &Pin_B5, 0x00007F13);
	
	// Init CAN bus
	can_init(BITRATE_500_KBPS);
	can_filter_t filter = {
        .id = 0x00007F00,
        .mask = 0x1FFFFF00,
        .flags = {
            .rtr = 0,
            .extended = 0
        }
    };
    can_set_filter(0, &filter);
	
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
				// TODO: Implement binary search tree
				if (AX12_OnMessage(&msg) == true) continue;
				if (RX24_OnMessage(&msg) == true) continue;
				if (MCP_OnMessage(&mcp, &msg) == true) continue;
				if (HBridge_OnMessage(&msg) == true) continue;
				if (FastPWM_OnMessage(&msg) == true) continue;
            }
        }
    }
	
	return 0;
}
