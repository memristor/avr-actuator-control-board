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
	AX12_InitAll(0x00007F00);

	/*	Binary Sensors Initalisation	*/
	BinarySensor_Add(&Pin_C6, 1);
	BinarySensor_Add(&Pin_A2, 2);
	BinarySensor_Add(&Pin_A3, 3);
	BinarySensor_Add(&Pin_A4, 4);
	BinarySensor_Add(&Pin_A5, 5);
	BinarySensor_Add(&Pin_A6, 6);
	BinarySensor_Add(&Pin_A7, 7);

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
