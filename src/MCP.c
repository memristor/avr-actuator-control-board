#include "MCP.h"


static MCPSensor sensors[MCP_CONFIG_SENSOR_MAX];
static MCPActuator actuators[MCP_CONFIG_ACTUATOR_MAX];
static uint8_t sensorsCount = 0;
static uint8_t actuatorsCount = 0;

//static uint8_t portState[MCP_CONFIG_MAX][2];

static void SPI_Write(unsigned char slaveAddress, unsigned char port, unsigned char data);
static unsigned char SPI_Read(unsigned char slaveAddress, unsigned char port);

void MCP_InitAll(MCP* mcp, volatile uint8_t* port, volatile uint8_t* ddr, uint8_t cs) {
	size_t i;
	
	// Set MOSI and SCK as output (and SS), others as input
	SPI_DDR = (1<<PB2)|(1<<PB1)|(1<<PB0);
	
	// CS pin is not active
	SPI_PORT |= (1<<SPI_CS);
	
	#if MCP23S17_EMULATION
		// Enable SPI, Master, set clock rate fck/64
		SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);
	#else
		// Enable SPI, Master, set clock rate fck/2
		SPCR = (1<<SPE)|(1<<MSTR);
		SPSR |= (1<<SPI2X);
	#endif
	
	// Initial the MCP23S17 SPI I/O Expanders
	for (i = 0; i < count; i++) {
		Debugger_SetLED(DEBUGGER_LED2, true);
		SPI_Write(slaveAddresses[i], IOCONA, 0x28);   // I/O Control Register: BANK=0, SEQOP=1, HAEN=1 (Enable Addressing)
	}
}

uint8_t MCP_AddActuator(uint8_t slaveAddress, uint8_t port, uint8_t p, uint16_t canId) {	
	uint8_t index = actuatorsCount;
		
	// Save properties
    actuators[index].port = port;
    actuators[index].slaveAddress = slaveAddress;
    actuators[index].p = p;
    actuators[index].canId = canId;
    
    // Initialize pins
    if (port == GPIOA) {
		SPI_Write(slaveAddress, IODIRA, 0x00);   // Set as Output
		SPI_Write(slaveAddress, GPIOA, 0xFF);    // Set default value to high
	}
    
    instancesCount++;
    return index;
}

inline void BinaryActuator_OnMessage(const can_t* const canMsg) {
	size_t i;
	for (i = 0; i < instancesCount; i++) {
		
		// Set HIGH at the pin if 0x01 is sent
		if (unlikely(canMsg->id == instaces[i].canId)) {
			// MCP works in inverted logic
			if (canMsg->data[0] == 0x01) {
				clear_bit(MCP1_PORTA_State, instaces[i].p);
			} else {
				set_bit(MCP1_PORTA_State, instaces[i].p);
			}
			SPI_Write(instaces[i].slaveAddress, GPIOA, MCP1_PORTA_State);
		}
	}
}


void SPI_Write(unsigned char slaveAddress, unsigned char port, unsigned char data) {
	// Activate the CS pin
	SPI_PORT &= ~(1<<SPI_CS);
	
	// Start MCP23S17 OpCode transmission
	SPDR = SPI_SLAVE_ID | ((slaveAddress << 1) & 0x0E)| SPI_SLAVE_WRITE;
	
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));
	
	// Start MCP23S17 Register Address transmission
	SPDR = port;
	
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));  

	// Start Data transmission
	SPDR = data;
	
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));
	
	// CS pin is not active
	SPI_PORT |= (1<<SPI_CS);
	
	// Set initial state 
	MCP1_PORTA_State = 0xFF;
	//SPI_Write(GPIOA, MCP1_PORTA_State);
}

unsigned char SPI_Read(unsigned char slaveAddress, unsigned char port)
{
	// Activate the CS pin
	SPI_PORT &= ~(1<<SPI_CS);
	// Start MCP23S17 OpCode transmission
	SPDR = SPI_SLAVE_ID | ((slaveAddress << 1) & 0x0E)| SPI_SLAVE_READ;
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));
	#if MCP23S17_EMULATION
	_delay_us(1);
	#endif
	// Start MCP23S17 Address transmission
	SPDR = port;
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));  

	#if MCP23S17_EMULATION
	_delay_us(1);
	#endif
	// Send Dummy transmission for reading the data
	SPDR = 0x00;
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));  

	// CS pin is not active
	SPI_PORT |= (1<<SPI_CS);
	return(SPDR);
}
