#include "BinaryActuator.h"

static void SPI_Write(unsigned char addr,unsigned char data);


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
	SPI_Write(IODIRB, 0xFF);   // GPIOB As Input
	SPI_Write(GPPUB, 0xFF);    // Enable Pull-up Resistor on GPIOB
	SPI_Write(GPIOA, 0x00);    // Reset Output on GPIOA
}

void SPI_Write(unsigned char addr,unsigned char data) {
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
}

inline void BinaryActuatorInit(
        BinaryActuator* binaryActuator,
        volatile uint8_t* ddr,
        volatile uint8_t* port,
        uint8_t pin,
        uint16_t canId
) {
	SPI_Write(0x07, 0xff);
	
    // Set pin as an output pin
    SET_BIT(*ddr, pin);

	// Store data abSPI_WriteReadout pins
    binaryActuator->port = port;
    binaryActuator->pin = pin;
    binaryActuator->canId = canId;
}

inline void BinaryActuatorProbe(BinaryActuator* binaryActuator, can_t* canMsg) {
	// Set HIGH at the pin if 0x01 is sent
    if (unlikely(canMsg->id == binaryActuator->canId)) {
        if (canMsg->data[0] == 0x01) {
            SET_BIT(*(binaryActuator->port), binaryActuator->pin);
        } else {
            CLEAR_BIT(*(binaryActuator->port), binaryActuator->pin);
        }
    }
}
