#ifndef _VACUUM_PUMP_H_
#define _VACUUM_PUMP_H_

#define VACUUM_PUMP_CONFIG_COUNT_MAX 5
#define VACUUM_SWITCH_RELASE_TIME 100

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <can/can_wrapper.h>
#include <util/delay.h>
#include "Utils.h"
#include "CanBus.h"
#include "Pin.h"

typedef struct _VacuumPump {
    Pin* pumpPin;
    Pin* switchPin;
    uint16_t canId;
} VacuumPump;

void VacuumPump_Add(Pin* pumpPin, Pin* switchPin, uint8_t number);
bool VacuumPump_OnMessage(can_t* canMsg);
bool SingleVacuumPump_OnMessage(can_t* canMsg);

#endif
