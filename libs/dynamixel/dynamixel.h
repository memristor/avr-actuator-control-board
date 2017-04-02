
#ifndef _DYNAMIXEL_H_
#define _DYNAMIXEL_H_

#include <avr/io.h>
#include <can/can_wrapper.h>

#define DYNAMIXEL_BAUDRATE 57142

#define DYNAMIXEL_OK      1
#define DYNAMIXEL_TIMEOUT 2

#define DYNAMIXEL_ID           2
#define DYNAMIXEL_LENGTH       3
#define DYNAMIXEL_INSTRUCTION  4
#define DYNAMIXEL_ERROR        4
#define DYNAMIXEL_PARAMETER    5

#define DYNAMIXEL_BROADCAST_ID  254
#define DYNAMIXEL_PACKET_SIZE   256

#define DYNAMIXEL_PING        1
#define DYNAMIXEL_READ        2
#define DYNAMIXEL_WRITE       3
#define DYNAMIXEL_REG_WRITE   4
#define DYNAMIXEL_ACTION      5
#define DYNAMIXEL_RESET       6
#define DYNAMIXEL_SYNC_WRITE  131

#define DYNAMIXEL_SUCCESS     1
#define DYNAMIXEL_RX_CORRUPT  2
#define DYNAMIXEL_RX_TIMEOUT  3
#define DYNAMIXEL_TX_FAIL     4
#define DYNAMIXEL_TX_TIMEOUT  5

#define DYNAMIXEL_RETRY_COUNT	10

#ifndef DYNAMIXEL_TIMEOUT_MS
// Timeout in milliseconds
#define DYNAMIXEL_TIMEOUT_MS 150
#endif


void dynamixel_ax_init(void);
void dynamixel_rx_init(void);

uint8_t dynamixel_ax_txrx(volatile uint8_t* txpacket, volatile uint8_t* rxpacket);
uint8_t dynamixel_rx_txrx(volatile uint8_t* txpacket, volatile uint8_t* rxpacket);


#endif
