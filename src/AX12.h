#ifndef _AX12_H_
#define _AX12_H_

#include <can/can.h>

void AX12_InitAll(uint16_t canId);
void AX12_OnMessage(can_t* canMsg);
#endif
