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
	BinarySensor_Add(&Pin_A1, 0x00007F21);
	BinarySensor_Add(&Pin_A0, 0x00007F22);
	BinarySensor_Add(&Pin_F3, 0x00007F23);
	BinarySensor_Add(&Pin_F2, 0x00007F24);

	// PWMs
	FastPWM_Add(&Pin_B7, 0x00007F12);
	Pin_WriteAnalog(&Pin_B7, 255); // Turn off by default

	// H-Bridges
	HBridge_Add(&Pin_C3, &Pin_C4, &Pin_B5, PIN_20KHz, 0x00007F13);
	HBridge_Add(&Pin_C0, &Pin_C1, &Pin_B4, PIN_7KHz, 0x00007F14);


	//Init_Pumps();

	//Init_Vacuume_Switches();
	/* Brushless EDF Initialisation on pin */
	//Brushless_Init(&Pin_B5);



	// Init CAN bus
	can_init(BITRATE_500_KBPS);
	can_filter_t filter = {
        .id = 0x00007F00,
        .mask = 0x0000FF00,
        .flags = {
            .rtr = 0,
            .extended = 0x03
        }
    };

    // For all CANPAGE-s
    can_set_filter(0, &filter);

	// Enable interrupts
    sei();

    while (1) {

			/*
					uart_char1 = USART0_receive();

					if (uart_char1 == 'c') {
						Stop_All_Pumps();
					} else if(uart_char1 == 'C') {
						Start_All_Pumps();
					} else if(uart_char1 == 'n') {
						Close_All_Vacuume_Switches();
					}	else if(uart_char1 == 'N') {
						Open_All_Vacuume_Switches();
					}

					//if (uart_char1 != 's' && uart_char1 != 'q') continue;
					if(!strchr("sqvV", uart_char1)) continue;

					uart_char2 = USART0_receive();

					if(uart_char2 >= '1' && uart_char2 <= '5') {

						uart_char2 -= 48;
						switch(uart_char1) {
							case 's':
									Start_Pump(uart_char2);
									break;
							case 'q':
									Stop_Pump(uart_char2);
									Open_Vacuume_Switch(uart_char2);
									_delay_ms(100);
									Close_Vacuume_Switch(uart_char2);
									break;
							case 'V':
									Open_Vacuume_Switch(uart_char2);
									break;
							case 'v':
									Close_Vacuume_Switch(uart_char2);
									break;
						}

					}

			*/

		// AX12_UpdateAll();
		BinarySensor_UpdateAll();

	can_wrapper_send(0x0000FFFF, 1, 0xFF);
        // Check if a new message was received
        if (can_check_message()) {
            can_t msg;

            // Try to read the message
            if (can_get_message(&msg)) {
				// can_wrapper_send(9999, 1, 1);
				// if (msg.id == 0xff) continue; // Ignore lidar messages

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
