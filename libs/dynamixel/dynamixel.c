
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#include "dynamixel.h"

volatile uint8_t dynamixel_txpacket[DYNAMIXEL_PACKET_SIZE];
volatile uint8_t dynamixel_rxpacket[DYNAMIXEL_PACKET_SIZE];
volatile uint8_t dynamixel_rxindex = 0;

ISR(USART0_RX_vect)
{
	dynamixel_rxpacket[dynamixel_rxindex++] = UDR0;
	
	// Ignore garbage
	if (dynamixel_rxindex == 2) {
		if (dynamixel_rxpacket[dynamixel_rxindex - 1] != 0xFF &&
			dynamixel_rxpacket[dynamixel_rxindex - 2] != 0xFF) {
			dynamixel_rxindex = 0;
		}
	}
}

void dynamixel_init(void)
{
	// Set UART baudrate
	UBRR0H = ((F_CPU / 16 + DYNAMIXEL_BAUDRATE / 2) / DYNAMIXEL_BAUDRATE - 1) >> 8;
	UBRR0L = ((F_CPU / 16 + DYNAMIXEL_BAUDRATE / 2) / DYNAMIXEL_BAUDRATE - 1);
	
	// Enable receiver and transmitter 
	UCSR0B |= (1 << TXEN0);
	UCSR0B |= (1 << RXEN0);
	UCSR0B |= (1 << RXCIE0);
	
	DDRE |= (1 << PE0);
	DDRE |= (1 << PE1);
	
	// Reset rx index
	dynamixel_rxindex = 0;
}

void dynamixel_settx(void)
{
	DDRE |= (1 << PE1);			// Set TX as output
	DDRE &= ~(1 << PE0);		// Set RX as input
	
	UCSR0B |= (1 << TXEN0);		// Enable TX
	UCSR0B &= ~(1 << RXEN0);	// Disable RX	
	UCSR0B &= ~(1 << RXCIE0);	// Disable RX interrupt
}


void dynamixel_setrx(void)
{	
	// Wait for TX complete flag before turning the bus around
	while(bit_is_clear(UCSR0A, TXC0));
	
	DDRE &= ~(1 << PE1);		// Set TX as input!
	DDRE |= (1 << PE0);			// Set RX as output
	
	UCSR0B &= ~(1 << TXEN0);	// Disable TX
	UCSR0B |= (1 << RXCIE0);	// Enable RX interrupt
	UCSR0B |= (1 << RXEN0);		// Enable RX	
	
	// Reset RX index
	dynamixel_rxindex = 0;
}

void dynamixel_write(uint8_t c)
{
	while(bit_is_clear(UCSR0A, UDRE0));
	UDR0 = c;
}

uint8_t dynamixel_calculatechecksum(volatile uint8_t* packet)
{
	uint16_t checksum = 0;
	
	for(uint8_t i = DYNAMIXEL_ID; i <= (packet[DYNAMIXEL_LENGTH] + 2); i++)
		checksum += packet[i];
	
	return ~(checksum % 256);
}

uint8_t dynamixel_writepacket(volatile uint8_t* txpacket, uint8_t packetlength)
{	
	for(uint8_t i = 0; i < packetlength; i++)
		dynamixel_write(txpacket[i]);
	
	return DYNAMIXEL_SUCCESS;
}

uint8_t dynamixel_readpacket(volatile uint8_t* rxpacket, uint8_t packetlength)
{
	uint16_t ulcounter = 0;
	size_t i;

	while(dynamixel_rxindex < packetlength)
	{
		if(ulcounter++ > (DYNAMIXEL_TIMEOUT_MS) * 10) {
			return DYNAMIXEL_RX_TIMEOUT;
		}
		_delay_us(100);
	}

	// Copy received packet to `rxpacket`
	for (i = 0; i < packetlength; i++) {
		rxpacket[i] = dynamixel_rxpacket[i];
	}
	
	// Check if packet is corrupted
	if((dynamixel_rxpacket[0] != 255) || (dynamixel_rxpacket[1] != 255))
		return DYNAMIXEL_RX_CORRUPT;
		
	if(dynamixel_rxpacket[packetlength - 1] != dynamixel_calculatechecksum(rxpacket))
		return DYNAMIXEL_RX_CORRUPT;
	
	return DYNAMIXEL_SUCCESS;
}

uint8_t dynamixel_txrx(volatile uint8_t* txpacket, volatile uint8_t* rxpacket)
{
	uint8_t rxlength = 0;
	uint8_t txlength = txpacket[DYNAMIXEL_LENGTH] + 4;
	
	txpacket[0] = (uint8_t) 0xff;
	txpacket[1] = (uint8_t) 0xff;
	txpacket[txlength - 1] = (uint8_t) dynamixel_calculatechecksum(txpacket);
		
	dynamixel_settx();
	dynamixel_writepacket(txpacket, txlength);
	dynamixel_setrx();
	
	if(txpacket[DYNAMIXEL_ID] != DYNAMIXEL_BROADCAST_ID)
	{	
		if(txpacket[DYNAMIXEL_INSTRUCTION] == DYNAMIXEL_READ)
			rxlength = txpacket[DYNAMIXEL_PARAMETER + 1] + 6;
		else
			rxlength = 6;

		return dynamixel_readpacket(rxpacket, rxlength);			
	}
	
	dynamixel_settx();
		
	return DYNAMIXEL_SUCCESS;
}

uint8_t dynamixel_ping(uint8_t id)
{	
	dynamixel_txpacket[DYNAMIXEL_ID]          = (uint8_t) id;
	dynamixel_txpacket[DYNAMIXEL_LENGTH]      = (uint8_t) 2;
	dynamixel_txpacket[DYNAMIXEL_INSTRUCTION] = (uint8_t) DYNAMIXEL_PING;
	
	return dynamixel_txrx(dynamixel_txpacket, dynamixel_rxpacket);
}

uint8_t dynamixel_readbyte(uint8_t id, uint8_t address, uint8_t* value)
{
	uint8_t result;
	
	dynamixel_txpacket[DYNAMIXEL_ID]          = (uint8_t) id;
	dynamixel_txpacket[DYNAMIXEL_LENGTH]      = (uint8_t) 4;
	dynamixel_txpacket[DYNAMIXEL_INSTRUCTION] = (uint8_t) DYNAMIXEL_READ;
	dynamixel_txpacket[DYNAMIXEL_PARAMETER]   = (uint8_t) address;
	dynamixel_txpacket[DYNAMIXEL_PARAMETER+1] = (uint8_t) 1;
	
	result = dynamixel_txrx(dynamixel_txpacket, dynamixel_rxpacket);
	
	if(result == DYNAMIXEL_SUCCESS)
		*value = dynamixel_rxpacket[DYNAMIXEL_PARAMETER];
	
	return result;
}

uint8_t dynamixel_readword(uint8_t id, uint8_t address, uint16_t* value)
{
	uint8_t result;
	
	dynamixel_txpacket[DYNAMIXEL_ID]          = (uint8_t) id;
	dynamixel_txpacket[DYNAMIXEL_LENGTH]      = (uint8_t) 4;
	dynamixel_txpacket[DYNAMIXEL_INSTRUCTION] = (uint8_t) DYNAMIXEL_READ;
	dynamixel_txpacket[DYNAMIXEL_PARAMETER]   = (uint8_t) address;
	dynamixel_txpacket[DYNAMIXEL_PARAMETER+1] = (uint8_t) 2;
	
	result = dynamixel_txrx(dynamixel_txpacket, dynamixel_rxpacket);
	
	if(result == DYNAMIXEL_SUCCESS)
		*value = (uint16_t) dynamixel_makeword(dynamixel_rxpacket[DYNAMIXEL_PARAMETER], dynamixel_rxpacket[DYNAMIXEL_PARAMETER+1]);
	
	return result;
}

uint8_t dynamixel_readtable(uint8_t id, uint8_t start_address, uint8_t end_address, uint8_t* table)
{
	uint8_t result;
	uint8_t length = end_address - start_address + 1;
	
	dynamixel_txpacket[DYNAMIXEL_ID]          = (uint8_t) id;
	dynamixel_txpacket[DYNAMIXEL_LENGTH]      = (uint8_t) 4;
	dynamixel_txpacket[DYNAMIXEL_INSTRUCTION] = (uint8_t) DYNAMIXEL_READ;
	dynamixel_txpacket[DYNAMIXEL_PARAMETER]   = (uint8_t) start_address;
	dynamixel_txpacket[DYNAMIXEL_PARAMETER+1] = (uint8_t) length;
	
	result = dynamixel_txrx(dynamixel_txpacket, dynamixel_rxpacket);
	
	if(result == DYNAMIXEL_SUCCESS)
	{
		for(uint8_t i = 0; i < length; i++)
			table[start_address + i] = dynamixel_rxpacket[DYNAMIXEL_PARAMETER + i];
	}
	
	return result;
}

uint8_t dynamixel_writebyte(uint8_t id, uint8_t address, uint8_t value)
{	
	dynamixel_txpacket[DYNAMIXEL_ID]          = (uint8_t) id;
	dynamixel_txpacket[DYNAMIXEL_LENGTH]      = (uint8_t) 4;
	dynamixel_txpacket[DYNAMIXEL_INSTRUCTION] = (uint8_t) DYNAMIXEL_WRITE;
	dynamixel_txpacket[DYNAMIXEL_PARAMETER]   = (uint8_t) address;
	dynamixel_txpacket[DYNAMIXEL_PARAMETER+1] = (uint8_t) value;
	
	return dynamixel_txrx(dynamixel_txpacket, dynamixel_rxpacket);
}

uint8_t dynamixel_writeword(uint8_t id, uint8_t address, uint16_t value)
{	
	dynamixel_txpacket[DYNAMIXEL_ID]          = (uint8_t) id;
	dynamixel_txpacket[DYNAMIXEL_LENGTH]      = (uint8_t) 5;
	dynamixel_txpacket[DYNAMIXEL_INSTRUCTION] = (uint8_t) DYNAMIXEL_WRITE;
	dynamixel_txpacket[DYNAMIXEL_PARAMETER]   = (uint8_t) address;
	dynamixel_txpacket[DYNAMIXEL_PARAMETER+1] = (uint8_t) dynamixel_getlowbyte(value);
	dynamixel_txpacket[DYNAMIXEL_PARAMETER+2] = (uint8_t) dynamixel_gethighbyte(value);
	
	return dynamixel_txrx(dynamixel_txpacket, dynamixel_rxpacket);
}

uint8_t dynamixel_syncwrite(uint8_t address, uint8_t length, uint8_t number, uint8_t* param)
{	
	dynamixel_txpacket[DYNAMIXEL_ID]          = (uint8_t) DYNAMIXEL_BROADCAST_ID;
	dynamixel_txpacket[DYNAMIXEL_INSTRUCTION] = (uint8_t) DYNAMIXEL_SYNC_WRITE;
	dynamixel_txpacket[DYNAMIXEL_PARAMETER]   = (uint8_t) address;
	dynamixel_txpacket[DYNAMIXEL_PARAMETER+1] = (uint8_t) length;
	dynamixel_txpacket[DYNAMIXEL_LENGTH]      = (uint8_t) ((length + 1) * number + 4);
	
	for(uint8_t i = 0; i < ((length + 1) * number); i++)
		dynamixel_txpacket[DYNAMIXEL_PARAMETER + 2 + i] = (uint8_t) param[i];
	
	return dynamixel_txrx(dynamixel_txpacket, dynamixel_rxpacket);
}

uint8_t dynamixel_reset(uint8_t id)
{
	dynamixel_txpacket[DYNAMIXEL_ID]          = (uint8_t) id;
	dynamixel_txpacket[DYNAMIXEL_LENGTH]      = (uint8_t) 2;
	dynamixel_txpacket[DYNAMIXEL_INSTRUCTION] = (uint8_t) DYNAMIXEL_RESET;
	
	return dynamixel_txrx(dynamixel_txpacket, dynamixel_rxpacket);
}

uint16_t dynamixel_makeword(uint8_t lowbyte, uint8_t highbyte)
{
	return ((highbyte << 8) + lowbyte);
}

uint8_t dynamixel_getlowbyte(uint16_t word)
{
	return (word & 0xff);
}

uint8_t dynamixel_gethighbyte(uint16_t word)
{
	return ((word & 0xff00) >> 8);
}