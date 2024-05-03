#include <ESP32Servo.h>

// WARN: Set all Pin Configuration and Setup related to esp32 Here

#define LDR_TOP_RIGHT 35
#define LDR_TOP_LEFT 34
#define LDR_BOTTOM_RIGHT 33
#define LDR_BOTTOM_LEFT 32

#define HORIZONTAL_SERVO 12
#define VERTICAL_SERVO 13

#define RELAY_PIN1 9
#define RELAY_PIN2 10

// Setting the LDR resistance Value thresholds
#define LDR_VALUE_HIGH 900;

#define LDR_VALUE_LOW 20;

// Servo Motor Rotation Values
#define SERVO_LIMIT_HIGH 180;
#define SERVO_LIMIT_LOW 10;
#define VERTICAL_SERVO_LIMIT_HIGH 80
#define VERTICAL_SERVO_LIMIT_LOW 20

// INFO: Multi Threadingza
//
// TaskHandle_t solar_tracking_id;
// TaskHandle_t energy_management_id;

// void SolarTracking(void *pvParameters);

Servo Horizontal_Servo;
Servo Vertical_Servo;

int avg(int pin_1, int pin_2);
void setup() {
  Serial.begin(9600);
  delay(50);
  analogReadResolution(10);
  delay(100);
  analogReadResolution(10);
  Horizontal_Servo.attach(HORIZONTAL_SERVO);
  Vertical_Servo.attach(VERTICAL_SERVO);
  delay(500);
}
void loop() {
  short V_current_position = 45;
  short H_current_position = 180;
  bool status = 1;
  // int avg_top, avg_bottom, avg_left, avg_right;

  while (status != 0) {

    if (avg(LDR_TOP_LEFT, LDR_TOP_RIGHT) >
        avg(LDR_BOTTOM_LEFT, LDR_BOTTOM_RIGHT)) {
      Vertical_Servo.write(40);
      V_current_position -= 5;
      V_current_position = (V_current_position < VERTICAL_SERVO_LIMIT_LOW)
                               ? VERTICAL_SERVO_LIMIT_LOW
                               : V_current_position;
    }
    if (avg(LDR_TOP_LEFT, LDR_TOP_RIGHT) >
        avg(LDR_BOTTOM_LEFT, LDR_BOTTOM_RIGHT)) {
      Vertical_Servo.write(80);
      V_current_position += 5;

      V_current_position = (V_current_position < VERTICAL_SERVO_LIMIT_HIGH)
                               ? VERTICAL_SERVO_LIMIT_HIGH
                               : V_current_position;
    }
    Serial.println("Diffrence in top and bottom = " +
                   String(avg(LDR_TOP_LEFT, LDR_TOP_RIGHT) -
                          avg(LDR_BOTTOM_LEFT, LDR_BOTTOM_RIGHT)));
    delay(3000);
    Serial.println("V_Current " + String(V_current_position));
    delay(300);
    delay(5);
    Serial.println("Avg Top " + String(avg(LDR_TOP_LEFT, LDR_TOP_RIGHT)));
    delay(1000);
    Serial.println("Avg Bottom" +
                   String(avg(LDR_BOTTOM_LEFT, LDR_BOTTOM_RIGHT)));
    delay(3000);
  }
}
int avg(int pin_1, int pin_2) {
  return (analogRead(pin_1) + analogRead(pin_2) / 2);
}
