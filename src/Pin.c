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

void Pin_EnableAnalog(Pin* pin, PinFrequency frequency) {
	switch (pin->timer->bits) {
		// Configuration for 8bit timers
		case 8:
			switch (frequency) {
				case PIN_7KHz:
					*(pin->timer->TCCRnA) |=
						(1 << pin->timer->WGMn0) |
						(1 << pin->timer->WGMn1) |
						(1 << pin->timer->COMnA1) |
						(1 << pin->timer->CSn1);
				break;
				case PIN_1KHz:
					*(pin->timer->TCCRnA) |=
					(1 << pin->timer->WGMn0) |
					(1 << pin->timer->WGMn1) |
					(1 << pin->timer->COMnA1) |
					(1 << pin->timer->CSn0) |
					(1 << pin->timer->CSn1);
				break;
				case PIN_66KHz:
					*(pin->timer->TCCRnA) |=
					(1 << pin->timer->WGMn0) |
					(1 << pin->timer->WGMn1) |
					(1 << pin->timer->COMnA1) |
					(1 << pin->timer->CSn0);
				break;
			}
			break;

		// Configuration for 16bit timers
		case 16:
			switch (frequency) {
				case PIN_66KHz:
					*(pin->timer->TCCRnA) |=
						(1 << pin->timer->WGMn0) |
						(1 << pin->timer->COMnA1);

					*(pin->timer->TCCRnB) |=
						(1 << pin->timer->CSn0) |
						(1 << pin->timer->WGMn2);

					clear_bit(*(pin->timer->TCCRnB), pin->timer->CSn1);
				break;

				case PIN_1KHz:
					*(pin->timer->TCCRnA) |=
						(1 << pin->timer->WGMn0) |
						(1 << pin->timer->COMnA1);

					*(pin->timer->TCCRnB) |=
						(1 << pin->timer->CSn0) |
						(1 << pin->timer->CSn1) |
						(1 << pin->timer->WGMn2);
				break;

				case PIN_20KHz:
					*(pin->timer->TCCRnA) |=
						(1 << pin->timer->WGMn1) |
						(1 << pin->timer->COMnA1);

					*(pin->timer->TCCRnB) |=
						(1 << pin->timer->CSn1) |
						(1 << pin->timer->WGMn2) |
						(1 << pin->timer->WGMn3);

					*(pin->timer->ICRn) = 100;
				break;
			}
			break;
	}
}

void Pin_WriteAnalog(Pin* pin, uint16_t value) {
	*(pin->timer->OCRnA) = value;
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
	if (bit_is_set(*(pin->PINx), pin->Pxn)) {
		return PIN_HIGH;
	} else {
		return PIN_LOW;
	}
}

// PB5
Timer Timer_1A = {
	.TCCRnA = &TCCR1A,
	.TCCRnB = &TCCR1B,
	.OCRnA = &OCR1A,
	.ICRn = &ICR1,
	.WGMn0 = WGM10,
	.WGMn1 = WGM11,
	.WGMn2 = WGM12,
	.WGMn3 = WGM13,
	.COMnA1 = COM1A1,
	.CSn0 = CS10,
	.CSn1 = CS11,
	.bits = 16
};

// PB7
Timer Timer_0A = {
	.TCCRnA = &TCCR0A,
	.OCRnA = &OCR0A,
	.ICRn = 0,
	.WGMn0 = WGM00,
	.WGMn1 = WGM01,
	.WGMn2 = 0,
	.WGMn3 = 0,
	.COMnA1 = COM0A1,
	.CSn0 = CS00,
	.CSn1 = CS01,
	.bits = 8
};

// PB4
Timer Timer_2A = {
	.TCCRnA = &TCCR2A,
	.OCRnA = &OCR2A,
	.ICRn = 0,
	.WGMn0 = WGM20,
	.WGMn1 = WGM21,
	.WGMn2 = 0,
	.WGMn3 = 0,
	.COMnA1 = COM2A1,
	.CSn0 = CS20,
	.CSn1 = CS21,
	.bits = 8
};

Pin Pin_A0 = {
	.DDRx = &DDRA,
	.PORTx = &PORTA,
	.PINx = &PINA,
	.Pxn = PA0,
	.timer = NULL
};

Pin Pin_A1 = {
	.DDRx = &DDRA,
	.PORTx = &PORTA,
	.PINx = &PINA,
	.Pxn = PA1,
	.timer = NULL
};

Pin Pin_A2 = {
	.DDRx = &DDRA,
	.PORTx = &PORTA,
	.PINx = &PINA,
	.Pxn = PA2,
	.timer = NULL
};

Pin Pin_A3 = {
	.DDRx = &DDRA,
	.PORTx = &PORTA,
	.PINx = &PINA,
	.Pxn = PA3,
	.timer = NULL
};

Pin Pin_A4 = {
	.DDRx = &DDRA,
	.PORTx = &PORTA,
	.PINx = &PINA,
	.Pxn = PA4,
	.timer = NULL
};

Pin Pin_A5 = {
	.DDRx = &DDRA,
	.PORTx = &PORTA,
	.PINx = &PINA,
	.Pxn = PA5,
	.timer = NULL
};

Pin Pin_A6 = {
	.DDRx = &DDRA,
	.PORTx = &PORTA,
	.PINx = &PINA,
	.Pxn = PA6,
	.timer = NULL
};

Pin Pin_A7 = {
	.DDRx = &DDRA,
	.PORTx = &PORTA,
	.PINx = &PINA,
	.Pxn = PA7,
	.timer = NULL
};


Pin Pin_B0 = {
	.DDRx = &DDRB,
	.PORTx = &PORTB,
	.PINx = &PINB,
	.Pxn = PB0,
	.timer = NULL
};

Pin Pin_B2 = {
	.DDRx = &DDRB,
	.PORTx = &PORTB,
	.PINx = &PINB,
	.Pxn = PB2,
	.timer = NULL
};

Pin Pin_B3 = {
	.DDRx = &DDRB,
	.PORTx = &PORTB,
	.PINx = &PINB,
	.Pxn = PB3,
	.timer = NULL
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

Pin Pin_B7 = {
	.DDRx = &DDRB,
	.PORTx = &PORTB,
	.PINx = &PINB,
	.Pxn = PB7,
	.timer = &Timer_0A
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

Pin Pin_C3 = {
	.DDRx = &DDRC,
	.PORTx = &PORTC,
	.PINx = &PINC,
	.Pxn = PC3,
	.timer = NULL
};

Pin Pin_C4 = {
	.DDRx = &DDRC,
	.PORTx = &PORTC,
	.PINx = &PINC,
	.Pxn = PC4,
	.timer = NULL
};

Pin Pin_C5 = {
	.DDRx = &DDRC,
	.PORTx = &PORTC,
	.PINx = &PINC,
	.Pxn = PC5,
	.timer = NULL
};

Pin Pin_C6 = {
	.DDRx = &DDRC,
	.PORTx = &PORTC,
	.PINx = &PINC,
	.Pxn = PC6,
	.timer = NULL
};

Pin Pin_C7 = {
	.DDRx = &DDRC,
	.PORTx = &PORTC,
	.PINx = &PINC,
	.Pxn = PC7,
	.timer = NULL
};


Pin Pin_E2 = {
	.DDRx = &DDRE,
	.PORTx = &PORTE,
	.PINx = &PINE,
	.Pxn = PE2,
	.timer = NULL
};

Pin Pin_E3 = {
	.DDRx = &DDRE,
	.PORTx = &PORTE,
	.PINx = &PINE,
	.Pxn = PE3,
	.timer = NULL
};


Pin Pin_F2 = {
	.DDRx = &DDRF,
	.PORTx = &PORTF,
	.PINx = &PINF,
	.Pxn = PF4,
	.timer = NULL
};

Pin Pin_F3 = {
	.DDRx = &DDRF,
	.PORTx = &PORTF,
	.PINx = &PINF,
	.Pxn = PF3,
	.timer = NULL
};


Pin Pin_G2 = {
	.DDRx = &DDRG,
	.PORTx = &PORTG,
	.PINx = &PING,
	.Pxn = PG2,
	.timer = NULL
};
