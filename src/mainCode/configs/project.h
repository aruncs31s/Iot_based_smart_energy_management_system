#include "pins.h"

unsigned short avg(unsigned short pin_1, unsigned short int pin_2) {
  return (analogRead(pin_1) + analogRead(pin_2) / 2);
}
