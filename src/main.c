#include <avr/io.h>
#include <util/delay.h>
#include "Config.h"
#include "Utils.h"
#include "CanBus.h"
#include "Uart.h"
#include "Pin.h"
#include "VacuumPumps.h"
#include "HBridge.h"
#include "AX12.h"

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
	
	AX12_InitAll(AX12_CANID);
	
//	VacuumPump_Add(&Pin_C1, &Pin_B4, 1);
	
	SpecialPump_Add(&Pin_A3, &Pin_C4, &Pin_C5,1);  
	SpecialPump_Add(&Pin_A2, &Pin_C3, &Pin_C2,2);
	SpecialPump_Add(&Pin_A4, &Pin_C1, &Pin_C0,3);
	SpecialPump_Add(&Pin_A3, &Pin_C4, &Pin_C5,7);  
	SpecialPump_Add(&Pin_A2, &Pin_C3, &Pin_C2,8);
	SpecialPump_Add(&Pin_A4, &Pin_C1, &Pin_C0,9);
	

	//HBridge_Add(&Pin_E2, &Pin_B5, &Pin_B2, PIN_20KHz, 1);
	
#ifdef DEBUG
	/* ALL Initialisations Passed and UART sends 'k' */
	USART0_transmit('k');
#endif

  while(1) {
	  

		
		if (can_check_message()) {
			can_t msg;

			if (can_get_message(&msg)) {


				//if(HBridge_OnMessage(&msg) == true) continue;
				//if(VacuumPump_OnMessage(&msg) == true) continue;
				//if(SingleVacuumPump_OnMessage(&msg) == true) continue;
				if(SingleSpecialPump_OnMessage(&msg)==true) continue;
				if(AX12_OnMessage(&msg)==true) continue;
			}
		}
  }

  return 0;
}
