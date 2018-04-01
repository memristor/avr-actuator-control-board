#define DEBUG

#include <avr/io.h>
#include <util/delay.h>
#include "Config.h"
#include "Utils.h"
#include "CanBus.h"
#include "Uart.h"
#include "Pin.h"
#include "VacuumPumps.h"



int main() {


#ifdef DEBUG
	char uart_char1, uart_char2;
	/* UART0 for DEBUG Initialisation */
	USART0_init(57600);
#endif

	sei();

	/* CANbus Initialisation */
	CANbus_Init();


	/*	Vacuum Pump and Vacuum Switches Initialisation	*/
	/*	Function:	VacuumPump_Add(&pumpPin, &switchPin, number)*/
	VacuumPump_Add(&Pin_C1, &Pin_B4, 1);
	VacuumPump_Add(&Pin_C2, &Pin_A1, 2);
	VacuumPump_Add(&Pin_C3, &Pin_A2, 3);
	VacuumPump_Add(&Pin_C4, &Pin_A4, 4);
	VacuumPump_Add(&Pin_C5, &Pin_A6, 5);



#ifdef DEBUG
	/* ALL Initialisations Passed and UART sends 'k' */
	USART0_transmit('k');
#endif

  while(1) {



		//can_wrapper_send(0x0000FFFF, 1, 0xFF);

		if (can_check_message()) {
			can_t msg;

			if (can_get_message(&msg)) {
				USART0_transmit('c');
				if(VacuumPump_OnMessage(&msg) == true) continue;

			}
		}
  }

  return 0;
}
