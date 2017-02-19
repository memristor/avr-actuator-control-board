#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>

typedef enum _Debugger_LED { 
	DEBUGGER_LED1 = 0, 
	DEBUGGER_LED2 
} Debugger_LED;

void Debugger_Init(void);

/**
 * @example Debugger_SetLED(DEBUGGER_LED1, true);
 */
void Debugger_SetLED(Debugger_LED led, bool on);

/**
 * @example Debugger_BlinkLED(DEBUGGER_LED1, 300);
 */
void Debugger_BlinkLED(Debugger_LED led, double period);

#endif
