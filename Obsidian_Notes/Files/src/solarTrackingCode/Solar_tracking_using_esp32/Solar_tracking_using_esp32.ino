/*
Author : Arun CS
Github : https://github.com/aruncs31s
*/

#include <ESP32Servo.h>

// Define the GPIO (2,15) pin connected to the servo
const int SERVO_PIN_1 = 2;
const int SERVO_PIN_2 = 15;
// Define the GPIO ADC Pins for LDR Sensors
const int LDR_LEFT_TOP = 36;
const int LDR_LEFT_BOTTOM = 39;
const int LDR_RIGHT_TOP = 34;
const int LDR_RIGHT_BOTTOM = 35;

// Setting the LDR resistance Value thresholds
const int LDR_VALUE_HIGH = 900;
const int LDR_VALUE_LOW = 20;

// Servo Motor Rotation Values
const int SERVO_LIMIT_HIGH = 180;
const int SERVO_LIMIT_LOW = 10;

int current_Servo_1_value = 0;

// Create a Servo object
Servo Servo_1; // Horizontal
Servo Servo_2; // Vertical

void setup() {
  Serial.begin(9600);

  // Attach the servo to the GPIO pin
  Servo_1.attach(SERVO_PIN_1);
  Servo_2.attach(SERVO_PIN_2);

  // Bring the servo to inital Position
  Servo_1.write(0);
  Servo_2.write(0);
  delay(500); // Need To find Out :)

  // Read The LDR Values
}
void loop() {

  int reading_LDR_left_bottom = analogRead(LDR_LEFT_BOTTOM);
  int reading_LDR_left_top = analogRead(LDR_RIGHT_TOP);
  int reading_LDR_right_top = analogRead(LDR_RIGHT_TOP);
  int reading_LDR_right_bottom = analogRead(LDR_RIGHT_BOTTOM);

  int reading_LDR_left_avg =
      (reading_LDR_left_top + reading_LDR_left_bottom) / 2;
  int reading_LDR_right_avg =
      (reading_LDR_right_bottom + reading_LDR_right_top) / 2;
  int reading_LDR_bottom_avg =
      (reading_LDR_right_bottom + reading_LDR_left_bottom) / 2;

  int reading_LDR_top_avg = (reading_LDR_right_top + reading_LDR_left_top) / 2;

  if (reading_LDR_top_avg < reading_LDR_bottom_avg) {
    Servo_2.write(current_Servo_1_value - 1);
    if (current_Servo_1_value > SERVO_LIMIT_HIGH) {
      current_Servo_1_value = SERVO_LIMIT_HIGH;
    }
    delay(8);
  } else if (reading_LDR_bottom_avg < reading_LDR_top_avg) {
    Servo_2.write(current_Servo_1_value + 1);
    if (current_Servo_1_value < SERVO_LIMIT_LOW) {

      current_Servo_1_value = SERVO_LIMIT_LOW;
    }
    delay(8);
  } else {
    Servo_2.write(current_Servo_1_value);
  }

  if (reading_LDR_right_avg < reading_LDR_left_avg) {
    Servo_1.write(current_Servo_1_value - 1);

    if (current_Servo_1_value > SERVO_LIMIT_HIGH) {
      current_Servo_1_value = SERVO_LIMIT_HIGH;
    }
    delay(8);
  } else if (reading_LDR_left_avg < reading_LDR_right_avg) {
    Servo_1.write(current_Servo_1_value + 1);

        if (current_Servo_1_value < SERVO_LIMIT_LOW) {
      current_Servo_1_value = LDR_VALUE_LOW;
    }
    delay(8);
  } else {
    Servo_1.write(current_Servo_1_value);
  }

  delay(50);
}
