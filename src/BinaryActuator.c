#include "BinaryActuator.h"

static void SPI_Write(unsigned char addr, unsigned char data);

void BinaryActuatorInitAll(void) {
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
	
	// Initial the MCP23S17 SPI I/O Expander
	SPI_Write(IOCONA, 0x28);   // I/O Control Register: BANK=0, SEQOP=1, HAEN=1 (Enable Addressing)
	SPI_Write(IODIRA, 0x00);   // GPIOA As Output
	SPI_Write(GPIOA, 0x00);    // Enable Pull-up Resistor on GPIOB
}

void SPI_Write(unsigned char addr, unsigned char data) {
	// Activate the CS pin
	SPI_PORT &= ~(1<<SPI_CS);
	
	// Start MCP23S17 OpCode transmission
	SPDR = SPI_SLAVE_ID | ((SPI_SLAVE_ADDR << 1) & 0x0E)| SPI_SLAVE_WRITE;
	
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));
	
	// Start MCP23S17 Register Address transmission
	SPDR = addr;
	
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

void BinaryActuatorInit(BinaryActuator* binaryActuator, uint8_t port, uint8_t p, uint16_t canId) {		
    binaryActuator->port = port;
    binaryActuator->p = p;
    binaryActuator->canId = canId;
}

inline void BinaryActuatorProbe(BinaryActuator* binaryActuator, can_t* canMsg) {
	// Set HIGH at the pin if 0x01 is sent
    if (unlikely(canMsg->id == binaryActuator->canId)) {
		// MCP works in inverted logic
        if (canMsg->data[0] == 0x01) {
            clear_bit(MCP1_PORTA_State, binaryActuator->p);
        } else {
            set_bit(MCP1_PORTA_State, binaryActuator->p);
        }
        SPI_Write(GPIOA, MCP1_PORTA_State);
        can_wrapper_send(0x20, 1, 1);
    }
}
