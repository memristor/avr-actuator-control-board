#include "PwmBrushless.h"

#define BRUSHLESS_ACC_COEF 20

static void PWM_SetDuty(uint16_t promil);

static uint8_t CurrentSpeed = 0;

void Brushless_Calibration(void) {

  PWM_SetDuty(100);
  _delay_ms(8000);
  PWM_SetDuty(50);
  _delay_ms(2000);

}

void Brushless_Init(Pin* pin) {

  Pin_SetMode(pin, PIN_OUTPUT);

  *(pin->timer->TCCRnA) |= (1 << pin->timer->COMnA1) | (1 << pin->timer->WGMn1);
  *(pin->timer->TCCRnB) |= (1 << pin->timer->WGMn3) | (1 << pin->timer->CSn1);
  *(pin->timer->ICRn) = MAX_OCR;

  PWM_SetDuty(50);
  

}

// duty cycle (0 pr-1000 pr)
void PWM_SetDuty(uint16_t promil) {

  if(promil > 100) {
    promil = 100;
  } else if(promil < 50) {
    promil = 50;
  }

  uint16_t duty = 20 * promil;

  OCR1A = duty;

}

// speed (50 pr - 100 pr)
bool Brushless_OnMessage(can_t* canMsg) {

  size_t i = 0;

  if(canMsg->id == BRUSHLESS_CANID) {

    uint8_t speed = canMsg->data[0];

    if(speed > CurrentSpeed) {
      for(i = CurrentSpeed; i < speed; i++) {
          PWM_SetDuty((uint16_t)(50 + (50.0 / 255.0) * i + 0.5));
          _delay_ms(BRUSHLESS_ACC_COEF);
        }
    } else if(speed < CurrentSpeed) {
      for(i = CurrentSpeed; i > speed; i--) {
        PWM_SetDuty((uint16_t)(50 + (50.0 / 255) * i));
        _delay_ms(BRUSHLESS_ACC_COEF);
      }
    } else {
      PWM_SetDuty((uint16_t)(50 + (50.0 / 255) * speed));
    }

    CurrentSpeed = speed;

    return true;
  }

  return false;
}
