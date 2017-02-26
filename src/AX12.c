#include "AX12.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <dynamixel/ax.h>
#include <dynamixel/dynamixel.h>
#include "Utils.h"

static uint16_t CAN_ID;
static uint64_t lastUpdate = 0;
		
// Should be imported to custom structure to do not check all AX12 in loop
static AX12 positionUpdateQueue[AX12_CONFIG_QUEUE_MAX];
static uint8_t positionUpdateQueueCount = 0;
	
static size_t i;
	
void AX12_UpdateAll(void) {
	for (i = 0; i < positionUpdateQueueCount; i++) {
		// If disabled do not check anything. `0` means don't check!
		if (likely(positionUpdateQueue[i].updatePeriodMs == 0)) {
			continue;
		}
		
		if (Utils_Mills() > positionUpdateQueue[i].lastUpdate + positionUpdateQueue[i].updatePeriodMs) {
			uint8_t status;
			uint16_t value;
			
			positionUpdateQueue[i].lastUpdate = Utils_Mills();
			
			status = dynamixel_readword(
				positionUpdateQueue[i].id, 
				AX_PRESENT_POSITION_L, 
				&value
			);
			
			if (abs(value - positionUpdateQueue[i].requiredPosition) <= positionUpdateQueue[i].tolerance) {
				// TODO: Send response
			}
		}
	}
}

void AX12_InitAll(uint16_t canId) {
	dynamixel_init();
	CAN_ID = canId;
}

void AX12_OnMessage(can_t* canMsg) {
	if (unlikely(canMsg->id == CAN_ID)) {
		uint8_t txpacket[11];
		uint8_t rxpacket[11];
		uint8_t status;
		
		// Send packet to AX12 (Brain -> AX12)
		memcpy(txpacket + 2, canMsg->data, canMsg->length);
		status = dynamixel_txrx(txpacket, rxpacket);
		
		// Get a response from AX12 (Brain <- AX12)
		can_t msg;
		msg.id = CAN_ID;
		msg.flags.rtr = 0;
		msg.flags.extended = 1;
		uint8_t packetKernelLength;
		if (status == DYNAMIXEL_SUCCESS) {
			packetKernelLength = rxpacket[3] + 2 - 1; // Length + (1 Byte ID) + (1 Byte Lenght) - (1 Byte Checksum)
			memcpy(msg.data, rxpacket + 2, packetKernelLength);
		} else {
			packetKernelLength = 1;
			msg.data[0] = status;
		}		
		msg.length = packetKernelLength;
		can_send_message(&msg);
	}
}
