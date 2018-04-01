#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>

// Calculate the value needed for 
// the CTC match value in OCR1A.
#define CTC_MATCH_OVERFLOW ((F_CPU / 1000) / 8) 

#define set_bit(PORT_NR,BIT) (PORT_NR |= (1<<BIT))
#define clear_bit(PORT_NR,BIT) (PORT_NR &= ~(1<<BIT))

#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)

void Utils_Init(void);

extern uint64_t Utils_Mills(void);

#endif
