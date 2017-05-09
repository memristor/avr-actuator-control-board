#ifndef _FAST_PWM_H_
#define _FAST_PWM_H_

#include <can/can.h>
#include <stdint.h>
#include <stdbool.h>
#include "Utils.h"
#include "Pin.h"
#include "Config.h"

#ifndef FAST_PWM_CONFIG_COUNT_MAX
#define FAST_PWM_CONFIG_COUNT_MAX 10
#warning "Max queue of PWM update is not redefined"
#endif

typedef struct _FastPWM {
	Pin* pin;
	uint16_t canId;
} FastPWM; 


/**
 * @example FastPWM_Add(&PinB5, 2000);
 */
void FastPWM_Add(Pin* pin, uint16_t canId);

bool FastPWM_OnMessage(can_t* canMsg);

#endif
