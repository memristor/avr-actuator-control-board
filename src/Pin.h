#ifndef _PIN_H_
#define _PIN_H_

#include <avr/io.h>
#include <stdlib.h>
#include "Utils.h"

typedef struct _Timer {
	volatile uint8_t* TCCRnx;
	volatile uint16_t* OCRnx;
	uint8_t WGMn0;
	uint8_t WGMn1;
	uint8_t COMnx1;
	uint8_t CSn0;
} Timer;

typedef struct _Pin {
	volatile uint8_t* DDRx;
	volatile uint8_t* PORTx;
	volatile uint8_t* PINx;
	uint8_t Pxn;
	Timer* timer;
} Pin;

typedef enum _PinMode {
	PIN_INPUT = 0,
	PIN_OUTPUT = 1
} PinMode;

typedef enum _PinValue {
	PIN_LOW = 0,
	PIN_HIGH = 1
} PinValue;

void Pin_SetMode(Pin* pin, PinMode mode);
void Pin_EnableAnalog(Pin* pin);
void Pin_WriteAnalog(Pin* pin, uint16_t value);
void Pin_WriteDigital(Pin* pin, PinValue value);
PinValue Pin_ReadDigital(Pin* pin);

extern Pin Pin_B5;
extern Pin Pin_B4;
extern Pin Pin_B0;
extern Pin Pin_A2;
extern Pin Pin_C6;
extern Pin Pin_C7;
extern Pin Pin_G2;
extern Pin Pin_A7;
extern Pin Pin_A6;
extern Pin Pin_A5;
extern Pin Pin_A4;
extern Pin Pin_A3;
extern Pin Pin_C0;
extern Pin Pin_C1;
extern Pin Pin_C2;

#endif  // _PIN_H_
