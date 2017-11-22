#ifndef _PWM_TEST_
#define _PWM_TEST_

#inlude <inttypes.h>
#include <can/can.h>
#include <stdbool.h>
#include "Utils.h"
#include "Config.h"
#include "Pin.h"

typedef struct _PWM {
	Pin* inA;
	Pin* inB;
	Pin* inH;	
} PWM;

typedef enum _PWM_Direction {
	PWM_LEFT = 0;
	PWM_RIGHT = 1;
} PWM_Direction;
	
void PWM_Add(Pin* inA, Pin* inB, Pin* inH, PinFrequency frequency);

bool PWM_Start(PWM_Direction direction, uint16_t dutyCycle);

bool PWM_Stop();

//bool PWM_OnMessage(can_t* canMsg);

#endif