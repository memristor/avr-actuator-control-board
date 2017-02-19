#ifndef _BINARY_SENSOR_H_
#define _BINARY_SENSOR_H_

#include <stdint.h>
#include <can/can.h>
#include <can/can_wrapper.h>
#include "Utils.h"

#ifndef BINARY_SENSOR_CONFIG_MAX
#define BINARY_SENSOR_CONFIG_MAX 10
#warning "Maximal number of binary sensors is not overriden"
#endif

typedef struct _BinarySensor {
    volatile uint8_t* pin;
    uint8_t p;
    uint16_t canId;
    uint8_t state;
} BinarySensor;


/**
 * Instance a new binary sensor and put it to binary sensor collection.
 * @example uint8_t sensorId = BinarySensor_Push(&DDRC, &PORTC, &PINC, PC6, 0x8000);
 * 
 * @param ddr
 * @param port
 * @param pin
 * @param p
 * @param canId
 */ 
extern uint8_t BinarySensor_Add(
	volatile uint8_t* ddr,
	volatile uint8_t* port,
	volatile uint8_t* pin,
	uint8_t p,
	uint16_t canId
);

/**
 * Check state of all binary sensors. Call this function in loop.
 * @example while (1) { BinarySensor_UpdateAll(); }
 */
extern void BinarySensor_UpdateAll(void);

#endif
