#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#include "dynamixel.h"

// General
static uint8_t calculatechecksum(volatile uint8_t* packet);
static uint8_t readpacket(
	volatile uint8_t* rxpacket, 
	uint8_t packetlength, 
	volatile uint8_t* recv_buffer,
	volatile uint8_t* recv_buffer_index
);

// AX-12 Specific
static volatile uint8_t ax_buffer[DYNAMIXEL_PACKET_SIZE];
static volatile uint8_t ax_buffer_index = 0;
static void ax_settx(void);
static void ax_setrx(void);
static void ax_write(uint8_t c);

// RX-24 Specific
static volatile uint8_t rx_buffer[DYNAMIXEL_PACKET_SIZE];
static volatile uint8_t rx_buffer_index = 0;
static void rx_write(uint8_t c);


ISR(USART0_RX_vect) {
	uint8_t data = UDR0;
	
	ax_buffer[ax_buffer_index] = data;
	if (ax_buffer_index < DYNAMIXEL_PACKET_SIZE - 1) {
		ax_buffer_index++;
	}
	
	// Ignore garbage
	if (ax_buffer_index == 1 && ax_buffer[0] != 0xFF) {
		ax_buffer_index = 0;
	}
	if (ax_buffer_index == 3 && 
		ax_buffer[0] == 0xFF && 
		ax_buffer[1] == 0xFF && 
		ax_buffer[2] == 0xFF) {
		ax_buffer_index = 2;
	}
}

ISR(USART1_RX_vect) {
	rx_buffer[rx_buffer_index] = UDR1;
	if (rx_buffer_index < DYNAMIXEL_PACKET_SIZE - 1) {
		rx_buffer_index++;
	}
	
	// Ignore garbage
	if (rx_buffer_index == 1 && rx_buffer[0] != 0xFF) {
		rx_buffer_index = 0;
	}
}

void dynamixel_ax_init(void) {
	// Set UART baudrate
	UBRR0H = ((F_CPU / 8 + DYNAMIXEL_BAUDRATE / 2) / DYNAMIXEL_BAUDRATE - 1) >> 8;
	UBRR0L = ((F_CPU / 8 + DYNAMIXEL_BAUDRATE / 2) / DYNAMIXEL_BAUDRATE - 1);
	UCSR0A |= (1 << U2X0);
	
	// Enable receiver and transmitter 
	UCSR0B |= (1 << TXEN0);
	UCSR0B |= (1 << RXEN0);
	UCSR0B |= (1 << RXCIE0);
	
	DDRE |= (1 << PE0);
	DDRE |= (1 << PE1);
	
	// Reset rx index
	ax_buffer_index = 0;
}

void dynamixel_rx_init(void) {
	// Set UART baudrate
	UBRR1H = ((F_CPU / 8 + DYNAMIXEL_BAUDRATE / 2) / DYNAMIXEL_BAUDRATE - 1) >> 8;
	UBRR1L = ((F_CPU / 8 + DYNAMIXEL_BAUDRATE / 2) / DYNAMIXEL_BAUDRATE - 1);
	UCSR1A |= (1 << U2X1);
	
	// Enable receiver and transmitter 
	UCSR1B |= (1 << TXEN1);
	UCSR1B |= (1 << RXEN1);
	UCSR1B |= (1 << RXCIE1);
	
	// Reset rx index
	rx_buffer_index = 0;
}

void ax_settx(void) {
	UCSR0B &= ~(1 << RXEN0);	// Disable RX
	
	DDRE |= (1 << PE1);			// Set TX as output
	DDRE &= ~(1 << PE0);		// Set RX as input
	
	UCSR0B |= (1 << TXEN0);		// Enable TX
		
	//UCSR0B &= ~(1 << RXCIE0);	// Disable RX interrupt
}


void ax_setrx(void) {	
	// Wait for TX complete flag before turning the bus around
	while(bit_is_clear(UCSR0A, TXC0));
	
	ax_buffer_index = 0;		// Reset RX index
	
	DDRE &= ~(1 << PE1);		// Set TX as input!
	DDRE |= (1 << PE0);			// Set RX as output
	
	UCSR0B &= ~(1 << TXEN0);	// Disable TX
	//UCSR0B |= (1 << RXCIE0);	// Enable RX interrupt
	UCSR0B |= (1 << RXEN0);		// Enable RX	
}

void ax_write(uint8_t c) {
	while(bit_is_clear(UCSR0A, UDRE0));
	UDR0 = c;
}

void rx_write(uint8_t c) {
	while(bit_is_clear(UCSR1A, UDRE1));
	UDR1 = c;
}

uint8_t calculatechecksum(volatile uint8_t* packet) {
	uint16_t checksum = 0;
	
	for(uint8_t i = DYNAMIXEL_ID; i <= (packet[DYNAMIXEL_LENGTH] + 2); i++) {
		checksum += packet[i];
	}
	
	return ~(checksum % 256);
}

uint8_t readpacket(
	volatile uint8_t* rxpacket, 
	uint8_t packetlength, 
	volatile uint8_t* recv_buffer,
	volatile uint8_t* recv_buffer_index
	) {
	uint16_t ulcounter = 0;
	size_t i;

	while(*recv_buffer_index < packetlength) {
		if(ulcounter++ > (DYNAMIXEL_TIMEOUT_MS) * 10) {
			return DYNAMIXEL_RX_TIMEOUT;
		}
		_delay_us(100);
	}

	// Copy received packet to `rxpacket`
	for (i = 0; i < packetlength; i++) {
		rxpacket[i] = recv_buffer[i];
	}

	// Check if packet is corrupted
	if((rxpacket[0] != 255) || (rxpacket[1] != 255)) {
		return DYNAMIXEL_RX_CORRUPT;
	}
		
	if(rxpacket[packetlength - 1] != calculatechecksum(rxpacket)) {
		return DYNAMIXEL_RX_CORRUPT;
	}
	
	return DYNAMIXEL_SUCCESS;
}

uint8_t dynamixel_ax_txrx(volatile uint8_t* txpacket, volatile uint8_t* rxpacket) {
	uint8_t rxlength = 0;
	uint8_t txlength = txpacket[DYNAMIXEL_LENGTH] + 4;
	uint8_t status = DYNAMIXEL_SUCCESS;
	
	txpacket[0] = (uint8_t) 0xff;
	txpacket[1] = (uint8_t) 0xff;
	txpacket[txlength - 1] = (uint8_t)calculatechecksum(txpacket);
	
	for (uint8_t i = 0; i < DYNAMIXEL_RETRY_COUNT; i++) {
		// Write packet	
		ax_settx();
		for(uint8_t i = 0; i < txlength; i++) {
			ax_write(txpacket[i]);
		}
		ax_setrx();
		
		
		// Read response
		if(txpacket[DYNAMIXEL_ID] != DYNAMIXEL_BROADCAST_ID) {	
			if(txpacket[DYNAMIXEL_INSTRUCTION] == DYNAMIXEL_READ) {
				rxlength = txpacket[DYNAMIXEL_PARAMETER + 1] + 6;
			}
			else {
				rxlength = 6;
			}
			
			status = readpacket(rxpacket, rxlength, ax_buffer, &ax_buffer_index);
			if (status == DYNAMIXEL_SUCCESS) {
				return status;
			}
		} else {
			return DYNAMIXEL_SUCCESS;
		}
	}
		
	return status;
}

uint8_t dynamixel_rx_txrx(volatile uint8_t* txpacket, volatile uint8_t* rxpacket) {
	uint8_t rxlength = 0;
	uint8_t txlength = txpacket[DYNAMIXEL_LENGTH] + 4;
	
	txpacket[0] = (uint8_t) 0xff;
	txpacket[1] = (uint8_t) 0xff;
	txpacket[txlength - 1] = (uint8_t)calculatechecksum(txpacket);
	
	// Write packet	
	for(uint8_t i = 0; i < txlength; i++) {
		rx_write(txpacket[i]);
	}
	
	// Read response
	if(txpacket[DYNAMIXEL_ID] != DYNAMIXEL_BROADCAST_ID) {	
		if(txpacket[DYNAMIXEL_INSTRUCTION] == DYNAMIXEL_READ) {
			rxlength = txpacket[DYNAMIXEL_PARAMETER + 1] + 6;
		}
		else {
			rxlength = 6;
		}

		return readpacket(rxpacket, rxlength, rx_buffer, &rx_buffer_index);
	}
		
	return DYNAMIXEL_SUCCESS;
}
