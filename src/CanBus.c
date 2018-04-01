#include "CanBus.h"

void CANbus_Init(void) {

  can_init(CANBUS_BITRATE);

  // CANbus filters Initialisation for id and mask
  can_filter_t filter = {
      .id = 0x00006C00,
      .mask = 0x0000FF00,
      .flags = {
          .rtr = 0,
          .extended = 0x03
      }
  };

  // For all CANPAGE-s
  can_set_filter(0, &filter);

}
