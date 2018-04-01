#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

#include "Uart.h"

#define CLOCK_FQ (16000000)

void USART0_init(uint32_t baud)
{
	uint32_t temp_baud = 0;

	temp_baud = (CLOCK_FQ / (8 * baud)) - 1;

	//Set baud rate
	UBRR0H = (unsigned char) (temp_baud >> 8);
	UBRR0L = (unsigned char) temp_baud;

	UCSR0A |= (1 << U2X0);

	UCSR0B |= (1 << RXEN0);	// Disable RX

	DDRE |= (1 << PE1);			// Set TX as output
	DDRE &= ~(1 << PE0);		// Set RX as input

	UCSR0B |= (1 << TXEN0);		//Enable TX
	UCSR0B &= ~(1 << UCSZ02);	//Data size

	// async, no parity, 1 stop bit, 8 bit data size
	UCSR0C &= ~(1 << 7);
	UCSR0C &= ~(1 << UMSEL0);
	UCSR0C &= ~(1 << UPM01) ;
	UCSR0C &= ~(1 << UPM00);
	UCSR0C &= ~(1 << USBS0);
	UCSR0C &= ~(1 << USBS0);
	UCSR0C |= (1 << UCSZ01);
	UCSR0C |= (1 << UCSZ00);

}

void USART0_transmit(unsigned char data)
{
	while ( ! ( UCSR0A & (1<<UDRE0))); //Wait for empty transmit buffer

	UDR0 = data; //Put data into buffer, sends the data

}

unsigned char USART0_receive(void)
{
	while ( ! (UCSR0A & (1<<RXC0)));

	return UDR0;
}

/*
void usart0_axpacket_send(t_axPacket* packet)
{
	USART0_transmit(packet->sync_1);
	USART0_transmit(packet->sync_2);
	USART0_transmit(packet->ax_id);
	USART0_transmit(packet->length);
	USART0_transmit(packet->instruction);

	int i;
	for(i=0; i<packet->length-2; i++)
	{
		USART0_transmit(packet -> parameters[i]);
	}

	USART0_transmit(packet->checksum);
}
*/
