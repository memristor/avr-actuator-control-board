#include <avr/io.h>
#include <util/delay.h>
#include "CanBus.h"
#include "Uart.h"
#include "Pin.h"
#include "Config.h"


int main() {


#ifdef DEBUG
	char uart_char1, uart_char2;

	/* UART0 for DEBUG Initialisation */
	USART0_init(57600);
#endif

	sei();

	/*	Binary Sensors Initalisation	*/
	BinarySensor_Add(&Pin_A4, 0x00007F0F);
	BinarySensor_Add(&Pin_A3, 0x00007F10);
	BinarySensor_Add(&Pin_A2, 0x00007F11);
	BinarySensor_Add(&Pin_A1, 0x00007F21);
	BinarySensor_Add(&Pin_A0, 0x00007F22);
	BinarySensor_Add(&Pin_F3, 0x00007F23);
	BinarySensor_Add(&Pin_F2, 0x00007F24);

	/* CANbus Initialisation */
	CANbus_Init();

  while(1) {

		BinarySensor_UpdateAll();

		if (can_check_message()) {
			can_t msg;

			if (can_get_message(&msg)) {



			}
		}
  }

  return 0;
}
