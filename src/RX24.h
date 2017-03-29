#ifndef _RX24_H_
#define _RX24_H_

#include <can/can.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef RX24_CONFIG_QUEUE_MAX
#define RX24_CONFIG_QUEUE_MAX 2
#warning "Max queue of AX12 update is not redefined"
#endif

void RX24_InitAll(uint16_t canId);
void RX24_OnMessage(can_t* canMsg);
#endif
