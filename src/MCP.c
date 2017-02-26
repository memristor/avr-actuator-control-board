#include "MCP.h"
#include "Utils.h"
#include <util/delay.h>

static void SPI_Write(MCP* mcp, uint8_t port, uint8_t data);
static uint8_t SPI_Read(MCP* mcp, uint8_t port);

void MCP_Init(MCP* mcp, uint8_t slaveAddress, volatile uint8_t* port, volatile uint8_t* ddr, uint8_t cs) {
	mcp->sensorsCount = 0;
	mcp->actuatorsCount = 0;
	mcp->ddrA = 0x00;	// GPIOA As Output
	mcp->ddrB = 0x00;
	mcp->stateA = 0xff;	// Set LOW as default
	mcp->stateB = 0xff;
	
	mcp->port = port;
	mcp->ddr = ddr;
	mcp->cs = cs;
	mcp->slaveAddress = slaveAddress;
	
	// Set MOSI and SCK as output (and SS), others as input
	*ddr = (1<<PB2)|(1<<PB1)|(1<<PB0);
	
	// CS pin is not active
	*port |= (1 << cs);
	
	#if MCP23S17_EMULATION
		// Enable SPI, Master, set clock rate fck/64
		SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);
	#else
		// Enable SPI, Master, set clock rate fck/2
		SPCR = (1<<SPE)|(1<<MSTR);
		SPSR |= (1<<SPI2X);
	#endif
	
	// Initial the MCP23S17 SPI I/O Expander
	// I/O Control Register: BANK=0, SEQOP=1, HAEN=1 (Enable Addressing)
	SPI_Write(mcp, IOCONA, 0x28);
	SPI_Write(mcp, IODIRA, mcp->ddrA);   
	SPI_Write(mcp, GPIOA, mcp->stateA);    
	SPI_Write(mcp, IODIRB, mcp->ddrB);   
	SPI_Write(mcp, GPIOB, mcp->stateB); 
}

uint8_t MCP_AddActuator(MCP* mcp, uint8_t port, uint8_t p, uint16_t canId) {	
	uint8_t index = mcp->actuatorsCount;
		
	// Save properties
    mcp->actuators[index].port = port;
    mcp->actuators[index].p = p;
    mcp->actuators[index].canId = canId;
    
    // Initialize pins
    if (port == GPIOA) {
		clear_bit(mcp->ddrA, p);
		set_bit(mcp->stateA, p);
		
		SPI_Write(mcp, IODIRA, mcp->ddrA);   // Set as Output
		SPI_Write(mcp, GPIOA, mcp->stateA);  // Set default value to high
	} else {
		clear_bit(mcp->ddrB, p); // Set as Output
		set_bit(mcp->stateB, p); // Set default value to high
		
		SPI_Write(mcp, IODIRB, mcp->ddrB);   
		SPI_Write(mcp, GPIOB, mcp->stateB);  
	}
    
    (mcp->actuatorsCount)++;
    return index;
}

void MCP_OnMessage(MCP* mcp, const can_t* const canMsg) {
	size_t i;
	
	for (i = 0; i < mcp->actuatorsCount; i++) {
		// Set HIGH at the pin if 0x01 is sent
		if (unlikely(canMsg->id == mcp->actuators[i].canId)) {
			uint8_t* currentPortState = (mcp->actuators[i].port == GPIOA) ? &(mcp->stateA) : &(mcp->stateB);
			
			// MCP works in inverted logic
			if (canMsg->data[0] == 0x01) {
				clear_bit((*currentPortState), mcp->actuators[i].p);
			} else {
				set_bit((*currentPortState), mcp->actuators[i].p);
			}
			SPI_Write(mcp, mcp->actuators[i].port, (*currentPortState));
		}
	}
}	

void SPI_Write(MCP* mcp, uint8_t port, uint8_t data) {
	// Activate the CS pin
	*(mcp->port) &= ~(1 << (mcp->cs));
	
	// Start MCP23S17 OpCode transmission
	SPDR = SPI_SLAVE_ID | ((mcp->slaveAddress << 1) & 0x0E) | SPI_SLAVE_WRITE;
	
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
	*(mcp->port) |= (1 << (mcp->cs));
	
	// Set initial state 
	//MCP1_PORTA_State = 0xFF;
	//SPI_Write(GPIOA, MCP1_PORTA_State);
}

uint8_t SPI_Read(MCP* mcp, uint8_t port) {
	// Activate the CS pin
	*(mcp->port) &= ~(1 << mcp->cs);
	// Start MCP23S17 OpCode transmission
	SPDR = SPI_SLAVE_ID | ((mcp->slaveAddress << 1) & 0x0E)| SPI_SLAVE_READ;
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
	*(mcp->port) |= (1 << mcp->cs);
	return(SPDR);
}
