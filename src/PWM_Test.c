#include "PWM_Test.h"

static PWM instances[5];
static uint8_t count = 0;

void PWM_Add(Pin* inA, Pin* inB, Pin* inH, PinFrequency frequency){
	instances[count].inA = inA;
	instances[count].inB = inB;
	instances[count].inH = inH;
	
	Pin_SetMode(inA, PIN_OUTPUT);
	Pin_SetMode(inB, PIN_OUTPUT);
	Pin_SetMode(inH, PIN_OUTPUT);
	Pin_EnableAnalog(inA, frequency);
	
	count++;
}

bool PWM_Start(PWM_Direction direction, uint16_t dutyCycle){
	size_t i;
	
	for(i = 0; i < count; i++){
		
		Pin_WriteDigital(instances[i].inH, PIN_HIGH);
		
		if(direction){
			Pin_WriteDigital(instances[i].inB, PIN_HIGH);
			Pin_WriteAnalog(instances[i].inA, dutyCycle);	
		} else {
			Pin_WriteDigital(insrances[i].inB, PIN_LOW);
			Pin_WriteAnalog(instances[i].inA, (255-dutyCycle));	//when turning in the oposite direction, because output voltage is difference between 0 or 1 and pwm duty
		}
	}
}

bool PWM_Stop(){
	size_t i;
	
	for(i = 0; i < count; i++){
		Pin_WriteDigital(instances[i].inH, PIN_LOW);
	}
}
