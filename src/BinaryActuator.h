#ifndef _BINARY_ACTUATOR_H_
#define _BINARY_ACTUATOR_H_

#ifndef BINARY_ACTUATOR_CONFIG_MAX
#define BINARY_ACTUATOR_CONFIG_MAX 10
#warning "Maximal number of binary actuators is not overriden"
#endif

// TODO: MCP related functions should be moved to another module. 
// TODO: Add support for multiple MCPs, ports and IO.

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <can/can.h>
#include <can/can_wrapper.h>
#include "Utils.h"

// Reference: http://www.ermicro.com/blog/?p=1050
#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define SPI_CS   PB0

// MCP23S17 SPI Slave Device
#define SPI_SLAVE_ID    0x40
#define SPI_SLAVE_WRITE 0x00
#define SPI_SLAVE_READ  0x01

// MCP23S17 Registers Definition for BANK=0 (default)
#define IODIRA 0x00
#define IODIRB 0x01
#define IOCONA 0x0A
#define GPPUA  0x0C
#define GPPUB  0x0D
#define GPIOA  0x12
#define GPIOB  0x13

// Define MCP23S17 Slave Emulation Mode:
//
//   0 - Real MCP23S17:
//       Only works on real Microchip MCP23S17 SPI I/O
//   1 - ATMega168 SPI Slave (MCP23S17 Emulation):
//       Both works on real MCP23S17 and ATMega168 Slave Mode
//
#define MCP23S17_EMULATION 1

enum BinaryActuatorError {
    BINARY_ACTUATOR_SUCCESS = 0,
    BINARY_ACTUATOR_ERROR
};

typedef struct _BinaryActuator {
    uint8_t port;
    uint8_t p;
    uint8_t slaveAddress;
    uint16_t canId;
} BinaryActuator;


/**
 * Initialize binary actuator module. It uses SPI to communicate to MCP and set state for switches. 
 * @example BinaryActuator_InitAll((uint8_t []) { 0x07, 0x00 }, 2);
 */
extern void BinaryActuator_InitAll(const uint8_t* const slaveAddresses, uint8_t count);

/**
 * Add new binary actuator to collection.
 * @ref http://www.atmel.com/webdoc/AVRLibcReferenceManual/FAQ_1faq_port_pass.html
 */
extern uint8_t BinaryActuator_Add(uint8_t slaveAddress, uint8_t port, uint8_t p, uint16_t canId);

/**
 * Try to update state of binary actuators. Call this function on message received.
 * @example if (can_get_message(&msg)) { BinaryActuator_OnMessage(&msg); }
 */
extern void BinaryActuator_OnMessage(const can_t* const canMsg);

#endif
