#ifndef _MCP_H_ 
#define _MCP_H_ 

#include <stdint.h>
#include <can/can.h>
#include "Debugger.h"

#ifndef MCP_CONFIG_SENSOR_MAX
#define MCP_CONFIG_SENSOR_MAX 5
#warning "Maximal number of MCP sensors is not overriden"
#endif


#ifndef MCP_CONFIG_ACTUATOR_MAX
#define MCP_CONFIG_ACTUATOR_MAX 10
#warning "Maximal number of MCP actuators is not overriden"
#endif

#ifndef MCP_CONFIG_MAX
#define MCP_CONFIG_MAX 2
#warning "Maximal number of MCPs is not overriden"
#endif

// Define MCP23S17 Slave Emulation Mode:
//
//   0 - Real MCP23S17:
//       Only works on real Microchip MCP23S17 SPI I/O
//   1 - ATMega168 SPI Slave (MCP23S17 Emulation):
//       Both works on real MCP23S17 and ATMega168 Slave Mode
//
#define MCP23S17_EMULATION 1

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


typedef struct _MCPSensor {
    uint8_t port;
    uint8_t p;
    uint16_t canId;
} MCPSensor;

typedef struct _MCPActuator {
    uint8_t port;
    uint8_t p;
    uint16_t canId;
} MCPActuator;

typedef struct _MCP { 
	volatile uint8_t* port;	// PORTB
	volatile uint8_t* ddr;	// DDRB
	uint8_t cs;		// PB0
	uint8_t slaveAddress;
	MCPActuator actuators[MCP_CONFIG_ACTUATOR_MAX];
	MCPSensor sensors[MCP_CONFIG_SENSOR_MAX];
	uint8_t sensorsCount;
	uint8_t actuatorsCount;
	uint8_t ddrA;
	uint8_t ddrB;
	uint8_t stateA;
	uint8_t stateB;
} MCP;

/**
 * Initialize binary actuator module. It uses SPI to communicate to MCP and set state for switches. 
 * @example MCP_Init(&mcp, 0x00, &PORTB, &DDRB, PB0);
 */
extern void MCP_Init(MCP* mcp, uint8_t slaveAddress, volatile uint8_t* port, volatile uint8_t* ddr, uint8_t cs);

/**
 * Add new binary actuator to collection.
 * @ref http://www.atmel.com/webdoc/AVRLibcReferenceManual/FAQ_1faq_port_pass.html
 */
extern uint8_t MCP_AddActuator(MCP* mcp, uint8_t port, uint8_t p, uint16_t canId);

extern uint8_t MCP_AddSensor(MCP* mcp, uint8_t port, uint8_t p, uint16_t canId);

/**
 * Try to update state of binary actuators. Call this function on message received.
 * @example if (can_get_message(&msg)) { MCP_OnMessage(&msg); }
 */
extern void MCP_OnMessage(MCP* mcp, const can_t* const canMsg);

/**
 * Call this function in a main loop only if you have at least one sensor.
 */
extern void MCP_UpdateAll(void);


#endif
