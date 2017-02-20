#ifndef _MCP_H_ 
#define _MCP_H_ 

#include <stdint.h>
#include <can/can.h>

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
    uint8_t slaveAddress;
    uint16_t canId;
} MCPSensor;

typedef struct _MCPActuator {
    uint8_t port;
    uint8_t p;
    uint8_t slaveAddress;
    uint16_t canId;
} MCPActuator;

typedef struct _MCP { 
	uint8_t* port;	// PORTB
	uint8_t* ddr;	// DDRB
	uint8_t cs;		// PB0
	MCPActuator actuators[10];
	MCPSensor sensors[10];
} MCP;

/**
 * Initialize binary actuator module. It uses SPI to communicate to MCP and set state for switches. 
 * @example MCP_InitAll((uint8_t []) { 0x07, 0x00 }, 2);
 */
extern void MCP_Init(MCP* mcp, volatile uint8_t* port, volatile uint8_t* ddr, uint8_t cs);

/**
 * Add new binary actuator to collection.
 * @ref http://www.atmel.com/webdoc/AVRLibcReferenceManual/FAQ_1faq_port_pass.html
 */
extern void MCP_AddActuator(uint8_t slaveAddress, uint8_t port, uint8_t p, uint16_t canId);

extern void MCP_AddSensor(uint8_t slaveAddress, uint8_t port, uint8_t p, uint16_t canId);

/**
 * Try to update state of binary actuators. Call this function on message received.
 * @example if (can_get_message(&msg)) { MCP_OnMessage(&msg); }
 */
extern void MCP_OnMessage(const can_t* const canMsg);

/**
 * Call this function in a main loop only if you have sensors.
 */
extern void MCP_UpdateAll(void);


#endif
