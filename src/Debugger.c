#include "Debugger.h"

#include "Utils.h"

uint8_t p[] = { PG1, PG0 };

void Debugger_Init(void) {
	// Init LED1, set as output
	set_bit(DDRG, p[0]);
	
	// Init LED2, set as output
	set_bit(DDRG, p[1]);
}

void Debugger_SetLED(Debugger_LED led, bool on) {
	if (on == true) {
		set_bit(PORTG, p[led]);
	} else {
		clear_bit(PORTG, p[led]);
	}
}

void Debugger_BlinkLED(Debugger_LED led, double period) {
	while (1) {
		set_bit(PORTG, p[led]);
		_delay_ms(200);
		clear_bit(PORTG, p[led]);
		_delay_ms(200);
	}
}
