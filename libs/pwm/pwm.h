// Reference: https://github.com/maxEmbedded/avr-code-gallery/blob/master/Timer%20-%20PWM%20-%20LED%20Brightness.c

#define pwm_init(timer_index,pin)\
	TCCR##timer_index## |= (1<<WGM##timer_index##0)|(1<<COM##timer_index##1)|(1<<WGM##timer_index##1)|(1<<CS##timer_index##0);\
	DDRB |= (1<<P##pin##);
	
