#include "Pin.h"


void Pin_SetMode(Pin* pin, PinMode mode) {
	if (mode == PIN_INPUT) {
		// Set pull-up resistor
		set_bit(*(pin->PORTx), pin->Pxn);

		// Set input pin
		clear_bit(*(pin->DDRx), pin->Pxn);
	} else {
		set_bit(*(pin->DDRx), pin->Pxn);
	}
}

void Pin_EnableAnalog(Pin* pin) {
	*(pin->timer->TCCRnx) |= 
		(1 << pin->timer->WGMn0) | 
		(1 << pin->timer->WGMn1) | 
		(1 << pin->timer->COMnx1) | 
		(1 << pin->timer->CSn0);
}

void Pin_WriteAnalog(Pin* pin, uint16_t value) {
	*(pin->timer->OCRnx) = value;
}

void Pin_WriteDigital(Pin* pin, PinValue value) {
	// TODO: Implement without condition
	if (value == PIN_HIGH) {
		set_bit(*(pin->PORTx), pin->Pxn);
	} else {
		clear_bit(*(pin->PORTx), pin->Pxn);
	}
}

PinValue Pin_ReadDigital(Pin* pin) {
	return bit_is_set(*(pin->PINx), pin->Pxn);
}

// PB5
Timer Timer_1A = {
	.TCCRnx = &TCCR1A,
	.OCRnx = &OCR1A,
	.WGMn0 = WGM10,
	.WGMn1 = WGM11,
	.COMnx1 = COM1A1,
	.CSn0 = CS10
};

// PB4
Timer Timer_2A = {
	.TCCRnx = &TCCR2A,
	.OCRnx = &OCR2A,
	.WGMn0 = WGM20,
	.WGMn1 = WGM21,
	.COMnx1 = COM2A1,
	.CSn0 = CS20
};

Pin Pin_B4 = {
	.DDRx = &DDRB,
	.PORTx = &PORTB,
	.PINx = &PINB,
	.Pxn = PB4,
	.timer = &Timer_2A
};

Pin Pin_B5 = {
	.DDRx = &DDRB,
	.PORTx = &PORTB,
	.PINx = &PINB,
	.Pxn = PB5,
	.timer = &Timer_1A
};

Pin Pin_B0 = {
	.DDRx = &DDRB,
	.PORTx = &PORTB,
	.PINx = &PINB,
	.Pxn = PB0,
	.timer = NULL
};

Pin Pin_A2 = {
	.DDRx = &DDRA,
	.PORTx = &PORTA,
	.PINx = &PINA,
	.Pxn = PA2,
	.timer = NULL
};

Pin Pin_C6 = {
	.DDRx = &DDRC,
	.PORTx = &PORTC,
	.PINx = &PINC,
	.Pxn = PC2,
	.timer = NULL
};

Pin Pin_C7 = {
	.DDRx = &DDRC,
	.PORTx = &PORTC,
	.PINx = &PINC,
	.Pxn = PC7,
	.timer = NULL
};

Pin Pin_G2 = {
	.DDRx = &DDRG,
	.PORTx = &PORTG,
	.PINx = &PING,
	.Pxn = PG2,
	.timer = NULL
};

Pin Pin_A7 = {
	.DDRx = &DDRA,
	.PORTx = &PORTA,
	.PINx = &PINA,
	.Pxn = PA7,
	.timer = NULL
};

Pin Pin_A6 = {
	.DDRx = &DDRA,
	.PORTx = &PORTA,
	.PINx = &PINA,
	.Pxn = PA6,
	.timer = NULL
};

Pin Pin_A5 = {
	.DDRx = &DDRA,
	.PORTx = &PORTA,
	.PINx = &PINA,
	.Pxn = PA5,
	.timer = NULL
};

Pin Pin_A4 = {
	.DDRx = &DDRA,
	.PORTx = &PORTA,
	.PINx = &PINA,
	.Pxn = PA4,
	.timer = NULL
};

Pin Pin_A3 = {
	.DDRx = &DDRA,
	.PORTx = &PORTA,
	.PINx = &PINA,
	.Pxn = PA3,
	.timer = NULL
};

Pin Pin_C0 = {
	.DDRx = &DDRC,
	.PORTx = &PORTC,
	.PINx = &PINC,
	.Pxn = PC0,
	.timer = NULL
};

Pin Pin_C1 = {
	.DDRx = &DDRC,
	.PORTx = &PORTC,
	.PINx = &PINC,
	.Pxn = PC1,
	.timer = NULL
};

Pin Pin_C2 = {
	.DDRx = &DDRC,
	.PORTx = &PORTC,
	.PINx = &PINC,
	.Pxn = PC2,
	.timer = NULL
};
