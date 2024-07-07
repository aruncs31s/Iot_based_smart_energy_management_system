#include "configs/configs.h"
#include "configs/pins.h"
#include "configs/project.h"
#include <ESP32Servo.h>
#include <ThingSpeak.h>
#include <WiFi.h>
// #include <WiFiClient.h>

Servo Vertical_Servo;
Servo Horizontal_Servo;

void Solar_Tracking();
void Energy_Management();

// PIN Decelerations
const int &tol = TOLERANCE;
const int &v_low = V_SERVO_LIMIT_LOW;
const int &v_high = V_SERVO_LIMIT_HIGH;
const int &h_low = H_SERVO_LIMIT_LOW;
const int &h_high = H_SERVO_LIMIT_HIGH;
short V_current_position = 45;
short H_current_position = 180;
float panel_voltage = 0;
const int &vref = REFERENCE_VOLTAGE;

// Energy Management
const int &led = RELAY_PIN_LED;
const int &fan = RELAY_PIN_FAN;
// Define sensor pin
const int &sensor = RADAR_PIN;
const int &ldr = LDR_PIN;

// NOTE: Multithreading Handlers

TaskHandle_t solar_tracking_id;
TaskHandle_t energy_management_id;

// TODO: Complete the code Thinkspeak

const char *ssid = "802";
const char *password = "12345678";
const char *server = "api.thingspeak.com";
const char *apiKey = "K2RSX4NAB1NP8MA9";
WiFiClient client;

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
  pinMode(ldr, INPUT);
  delay(3000);
  pinMode(15, OUTPUT);
  pinMode(2, OUTPUT);  // For Energy Management Status
  pinMode(15, OUTPUT); // For Solar Tracking Status
  pinMode(4, OUTPUT);  // For Main Loop Status
  // NOTE: Configs related to Energy_Mangement

  pinMode(RADAR_PIN, INPUT);
  pinMode(25, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected To " + String(ssid));
  ThingSpeak.begin(client);
  delay(3000);

  Serial.println("Finished Configuring");

  delay(500);
}
void loop() {
  digitalWrite(4, HIGH);
  Solar_Tracking();
  Energy_Management();
  delay(300);
  digitalWrite(4, LOW);
}

void Solar_Tracking() {
  digitalWrite(15, HIGH);
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


  panel_voltage = measure_voltage(39, vref);
  Serial.print("Solar Panel Voltage = ");
  Serial.println(panel_voltage, 5);

  delay(1);
  int status = ThingSpeak.writeField(2537564, 1, panel_voltage, apiKey);
  delay(25);
  if (status == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("HTTP error code " + String(status));
  }
  digitalWrite(15, LOW);
}

void Energy_Management() {
  digitalWrite(2, HIGH);
  // Fan Control
  Serial.println("Executing Energy Management");
  Serial.println("Radar Sensor Value : " + String(digitalRead(sensor)));
  if (digitalRead(sensor) == HIGH) {
    Serial.println("Motion Detected ");
    digitalWrite(fan, HIGH);
    Serial.println("Truning on the FAN");
    int status = ThingSpeak.writeField(2537564, 2, 1, apiKey);
        delay(100);
    if (status == 200) {
      Serial.println("Channel update successful.");
    } else {
      Serial.println("HTTP error code " + String(status));
    }

  } else {
    digitalWrite(fan, LOW); // turn relay OFF (light OFF)
    Serial.println("No Motion Detected ");
    Serial.println("Turning off the FAN");
    int status = ThingSpeak.writeField(2537564, 2, 0, apiKey);
    delay(25);
    if (status == 200) {
      Serial.println("Channel update successful.");
    } else {
      Serial.println("HTTP error code " + String(status));
    }
  }
  // Light Controll
  Serial.println(analogRead(39));

  if (analogRead(ldr) >= 3600) {
    digitalWrite(led, LOW);
    Serial.println("Light Intensity is High");

    Serial.println("Turing off the Light");

    int status = ThingSpeak.writeField(2537564, 3, 0, apiKey);

    delay(25);
    if (status == 200) {
      Serial.println("Channel update successful.");
    } else {
      Serial.println("HTTP error code " + String(status));
    }

  } else {
    Serial.println("Light Intensity is Low");
    digitalWrite(led, HIGH);
    Serial.println("Turing On the Light");

    int status = ThingSpeak.writeField(2537564, 3, 1, apiKey);
    delay(25);
    if (status == 200) {
      Serial.println("Channel update successful.");
    } else {
      Serial.println("HTTP error code " + String(status));
    }
  }
  //
  digitalWrite(2, LOW);
  delay(1);
}
