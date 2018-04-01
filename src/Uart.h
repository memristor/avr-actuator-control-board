/**
 * @file usart0.h
 */
#include <stdio.h>

#ifndef USART0_H_
#define USART0_H_

/**
 *
 * Uart0 configuring for communication with
 * dynamixel ax-12a servo motor.
 * @param desired baud rate, double speed mode
 */
void USART0_init(uint32_t baud);

void USART0_transmit(unsigned char data);
unsigned char USART0_receive(void);
/*
void usart0_axpacket_send(t_axPacket* packet);
*/

#endif
