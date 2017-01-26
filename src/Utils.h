#define set_bit(PORT_NR,BIT) (PORT_NR |= (1<<BIT))
#define clear_bit(PORT_NR,BIT) (PORT_NR &= ~(1<<BIT))

#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)
