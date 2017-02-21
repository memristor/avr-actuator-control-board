#ifndef _AX12_H_
#define _AX12_H_

#include <can/can.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef AX12_CONFIG_QUEUE_MAX
#define AX12_CONFIG_QUEUE_MAX 10
#warning "Max queue of AX12 update is not redefined"
#endif

typedef struct _AX12 {
	uint64_t epochStart;
	uint64_t lastUpdate;
	uint8_t id;
	uint8_t updatePeriodMs;
	uint8_t tolerance;
	uint16_t requiredPosition;
	uint16_t timeoutMs;
} AX12;

void AX12_InitAll(uint16_t canId);
void AX12_OnMessage(can_t* canMsg);
void AX12_UpdateAll(void);
#endif
