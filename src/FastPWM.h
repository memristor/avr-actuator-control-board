#ifndef _FAST_PWM_H_
#define _FAST_PWM_H_

#include <can/can.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef FAST_PWM_CONFIG_COUNT_MAX
#define FAST_PWM_CONFIG_COUNT_MAX 10
#warning "Max queue of PWM update is not redefined"
#endif

typedef struct _FastPWM {
	volatile uint8_t* ocr;
	uint16_t canId;
} FastPWM; 


/**
 * @example FastPWM_Add(&TCCR0A, &OCR0A, &DDRB, PB7, 0x2000);
 */
uint8_t FastPWM_Add(volatile uint8_t* counter, volatile uint8_t* ocr, volatile uint8_t* ddr, uint8_t p, uint16_t canId);

void FastPWM_OnMessage(can_t* canMsg);

#endif
