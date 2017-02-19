#include "BinaryActuator.h"

static BinaryActuator instaces[BINARY_ACTUATOR_CONFIG_MAX];
static uint8_t instancesCount = 0;

static uint8_t MCP1_PORTA_State;
static void SPI_Write(unsigned char slaveAddress, unsigned char port, unsigned char data);

void BinaryActuator_InitAll(const uint8_t* const slaveAddresses, uint8_t count) {
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
		SPI_Write(slaveAddresses[i], IOCONA, 0x28);   // I/O Control Register: BANK=0, SEQOP=1, HAEN=1 (Enable Addressing)
		SPI_Write(slaveAddresses[i], IODIRA, 0x00);   // GPIOA As Output
		SPI_Write(slaveAddresses[i], GPIOA, 0x00);    // Enable Pull-up Resistor on GPIOB
	}
}

uint8_t BinaryActuator_Add(uint8_t slaveAddress, uint8_t port, uint8_t p, uint16_t canId) {	
	uint8_t index = instancesCount;
		
    instaces[index].port = port;
    instaces[index].slaveAddress = slaveAddress;
    instaces[index].p = p;
    instaces[index].canId = canId;
    
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

