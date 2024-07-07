/* Author : Arun CS
 * Github : https://github/.com/aruncs31ss
 */
#include "pins.h"

unsigned short avg(unsigned short pin_1, unsigned short int pin_2) {
  return (analogRead(pin_1) + analogRead(pin_2) / 2);
}
float measure_voltage(short pin, int og_voltage) {
	int analogValue = analogRead(pin) ;
  return ((float)analogValue / 4095) * og_voltage;
}
