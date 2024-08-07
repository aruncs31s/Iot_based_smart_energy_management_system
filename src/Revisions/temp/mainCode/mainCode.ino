#include "configs/configs.h"
#include "configs/pins.h"
#include "configs/project.h"
#include <ESP32Servo.h>

Servo Vertical_Servo;
Servo Horizontal_Servo;

void Solar_Tracking();
void Energy_Mangement();

// PIN Decelerations
const int tol = 50;
const int &v_low = V_SERVO_LIMIT_LOW;
const int &v_high = V_SERVO_LIMIT_HIGH;
const int &h_low = H_SERVO_LIMIT_LOW;
const int &h_high = H_SERVO_LIMIT_HIGH;
short V_current_position = 45;
short H_current_position = 180;

// Energy Management
const int &led = RELAY_PIN_LED;
const int &fan = RELAY_PIN_FAN;
// Define sensor pin
const int &sensor = RADAR_PIN;
const int &ldr = LDR_PIN;

// NOTE: Multithreading Handlers

TaskHandle_t solar_tracking_id;
TaskHandle_t energy_management_id;




void setup() {
  Serial.begin(9600);
  // analogReadResolution(10);
  delay(50);

  Vertical_Servo.attach(VERTICAL_SERVO_PIN);
  Horizontal_Servo.attach(HORIZONTAL_SERVO_PIN);

  // For Energy_Mangement
  pinMode(led, OUTPUT);   // initialize relay as an output
  pinMode(fan, OUTPUT);   // initialize relay as an output
  pinMode(sensor, INPUT); // initialize sensor as an input
                          //
  delay(3000);

  // NOTE: Configs related to Energy_Mangement

  pinMode(RADAR_PIN, INPUT);

  Serial.println("Finished Configuring");
  delay(500);
}
void loop() {
  Solar_Tracking();
  Energy_Management();
}

void Solar_Tracking() {
  Serial.println("Executing Solar Tracking");

  Vertical_Servo.write(V_current_position);
  Horizontal_Servo.write(H_current_position);

  int avg_top = avg(LDR_TOP_RIGHT, LDR_TOP_LEFT);
  int avg_bottom = avg(LDR_BOTTOM_LEFT, LDR_BOTTOM_RIGHT);
  int avg_left = avg(LDR_TOP_LEFT, LDR_BOTTOM_LEFT);
  int avg_right = avg(LDR_TOP_RIGHT, LDR_BOTTOM_RIGHT);

  int diff_vert = avg_top - avg_bottom; // check the difference of up and down
  int diff_horiz =
      avg_left - avg_right; // check the difference of left and right
  if (-1 * tol > diff_vert || diff_vert > tol) {
    if (avg_top < avg_bottom) {
      V_current_position++;
      if (V_current_position > v_high) {
        V_current_position = V_SERVO_LIMIT_HIGH;
      }
    } else if (avg_top > avg_bottom) {
      V_current_position--;
      if (V_current_position < v_low) {
        V_current_position = V_SERVO_LIMIT_LOW;
      }
    }
    Vertical_Servo.write(V_current_position);
  }

  if (-1 * tol > diff_horiz || diff_horiz > tol) {
    if (avg_left < avg_right) {
      H_current_position++;
      if (H_current_position > h_high) {
        H_current_position = H_SERVO_LIMIT_HIGH;
      }
    }
    if (avg_left > avg_right) {
      H_current_position--;
      if (H_current_position < h_low) {
        H_current_position = H_SERVO_LIMIT_LOW;
      }
    }
    Horizontal_Servo.write(H_current_position);
  }
  }
// int Energy_Mangement(){

// }
//
void Energy_Management() {
  // Fan Controll
  Serial.println("Executing Energy Management");
  Serial.println("Radar Sensor Value : " + String(digitalRead(sensor)));
  if (digitalRead(sensor) == HIGH) {
    digitalWrite(fan, HIGH);
    Serial.println("Truning on the FAN");
  } else {
    digitalWrite(fan, LOW); // turn relay OFF (light OFF)
    Serial.println("Turning off the FAN");
  }
  // Light Controll
  Serial.println("LDR Value of EM : " + String(analogRead(ldr)));

  if (analogRead(ldr) >= 3600) {
    digitalWrite(led, LOW);
    Serial.println("Light OFF");
  } else {
    digitalWrite(led, HIGH);
    Serial.println("Light ON");
  }
  //
  delay(1);
}
