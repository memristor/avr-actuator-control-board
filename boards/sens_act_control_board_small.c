#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "Config.h"
#include "Pin.h"
#include "BinarySensor.h"
#include "CanBus.h"
#include "Uart.h"
#include "AX12.h"
#include "PwmBrushless.h"

int main() {


#ifdef DEBUG
	char uart_char1, uart_char2;

	/* UART0 for DEBUG Initialisation */
	USART0_init(57600);
#endif

	sei();

	/*	AX12 Servos Initialisation	*/
	AX12_InitAll(0x8d00);

	/*	Binary Sensors Initalisation	*/
	BinarySensor_Add(&Pin_C6, 0x8d01);
	BinarySensor_Add(&Pin_A2, 0x8d02);
	BinarySensor_Add(&Pin_A3, 0x8d03);
	BinarySensor_Add(&Pin_A4, 0x8d04);
	BinarySensor_Add(&Pin_A5, 0x8d05);
	BinarySensor_Add(&Pin_A6, 0x8d06);
	BinarySensor_Add(&Pin_A7, 0x8d07);
	BinarySensor_Add(&Pin_F5, 0x8d08);
	BinarySensor_Add(&Pin_F4, 0x8d09);

	/* Brushless EDF Initialisation on pin */
	Brushless_Init(&Pin_B5);


	/* CANbus Initialisation */
	CANbus_Init();

#ifdef DEBUG
	/* ALL Initialisations Passed and UART sends 	'k' */
	USART0_transmit('k');
#endif

  while(1) {

		BinarySensor_UpdateAll();

		if (can_check_message()) {
			can_t msg;

			if (can_get_message(&msg)) {
				if(Brushless_OnMessage(&msg) == true) continue;
				if(AX12_OnMessage(&msg) == true) continue;

			}
		}
  }

  return 0;
}
