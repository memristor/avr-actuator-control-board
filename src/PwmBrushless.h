#ifndef _PWM_BRUSHLESS_H_
#define _PWM_BRUSHLESS_H_

#define MAX_OCR 20000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "CanBus.h"
#include "Pin.h"
#include "Config.h"

void Brushless_Calibration(void);

void Brushless_Init(Pin* pin);

bool Brushless_OnMessage(can_t* canMsg);

#endif
