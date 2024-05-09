/* Author : Arun CS
 * Github : https://github/.com/aruncs31ss
 */
#include "pins.h"

unsigned short avg(unsigned short pin_1, unsigned short int pin_2) {
  return (analogRead(pin_1) + analogRead(pin_2) / 2);
}
float measure_voltage(short pin, int og_voltage) {
  Serial.println(analogRead(pin));
  return ((float)analogRead(pin) / 4095) * og_voltage;
}
struct ThingSpeakData {
  int value;
  short channelNumber;
};
// float measure_current() {}
