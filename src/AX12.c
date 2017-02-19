#include "AX12.h"

#include <string.h>
#include <dynamixel/ax.h>
#include <dynamixel/dynamixel.h>
#include "Utils.h"

static uint16_t CAN_ID;

void AX12_InitAll(uint16_t canId) {
	dynamixel_init();
	CAN_ID = canId;
}

void AX12_OnMessage(can_t* canMsg) {
	if (unlikely(canMsg->id == CAN_ID)) {
		uint8_t txpacket[11];
		uint8_t rxpacket[11];
		
		// Send packet to AX12 (Brain -> AX12)
		memcpy(txpacket + 2, canMsg->data, canMsg->length);
		dynamixel_txrx(txpacket, rxpacket);
		
		// Get a response from AX12 (Brain <- AX12)
		can_t msg;
		msg.id = CAN_ID;
		msg.flags.rtr = 0;
		msg.flags.extended = 1;
		msg.length = 8;
        memcpy(msg.data, rxpacket, 8); // TODO: Slice packet if needed
		can_send_message(&msg);
	}
}
