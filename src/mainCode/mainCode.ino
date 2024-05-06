#include "configs/configs.h"
#include "configs/pins.h"
#include "configs/project.h"
#include <ESP32Servo.h>
// #include <WiFi.h>
#include <ThingSpeak.h>
#include <WiFiClient.h>

Servo Vertical_Servo;
Servo Horizontal_Servo;

void wifi_connect();
void Solar_Tracking();
void Energy_Mangement();
// PIN Decelerations
const int &tol = TOLERANCE;
const int &v_low = V_SERVO_LIMIT_LOW;
const int &v_high = V_SERVO_LIMIT_HIGH;
const int &h_low = H_SERVO_LIMIT_LOW;
const int &h_high = H_SERVO_LIMIT_HIGH;

// Energy Management
const int &led = RELAY_PIN_LED;
const int &fan = RELAY_PIN_FAN;
// Define sensor pin
const int &sensor = RADAR_PIN;
const int &ldr = LDR_PIN;

// TODO: Complete the code Thinkspeak
const char *server = "api.thingspeak.com";
const char *apiKey = "YOUR_THINGSPEAK_API_KEY";
WiFiClient WiFi;

void setup() {
  Serial.begin(9600);
  analogReadResolution(10);
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
  wifi_connect();

  Serial.println("Finished Configuring");
}
void loop() {
  // Solar_Tracking();
  Energy_Management();
}

void Solar_Tracking() {
  Serial.println("Executing Solar Tracking");
  short V_current_position = 45;
  short H_current_position = 180;
  Vertical_Servo.write(V_current_position);
  Horizontal_Servo.write(H_current_position);

  int avg_top = avg(LDR_TOP_RIGHT, LDR_TOP_LEFT);
  int avg_bottom = avg(LDR_TOP_RIGHT, LDR_TOP_LEFT);
  int avg_left = avg(LDR_TOP_RIGHT, LDR_TOP_LEFT);
  int avg_right = avg(LDR_TOP_RIGHT, LDR_TOP_LEFT);

  int diff_vert = avg_top - avg_bottom; // check the difference of up and down
  int diff_horiz =
      avg_left - avg_right; // check the difference of left and right
  if (-1 * tol > diff_vert || diff_vert > tol) {
    if (avg_top > avg_bottom) {
      V_current_position++;
      if (V_current_position > v_high) {
        V_current_position = V_SERVO_LIMIT_HIGH;
      }
    } else if (avg_top < avg_bottom) {
      V_current_position--;
      if (V_current_position < v_low) {
        V_current_position = V_SERVO_LIMIT_LOW;
      }
    }
    Vertical_Servo.write(V_current_position);
  }

  if (-1 * tol > diff_horiz || diff_horiz > tol) {
    if (avg_left > avg_right) {
      H_current_position++;
      if (H_current_position > h_high) {
        H_current_position = H_SERVO_LIMIT_HIGH;
      }
    }
    if (avg_left < avg_right) {
      H_current_position--;
      if (H_current_position < h_low) {
        H_current_position = H_SERVO_LIMIT_LOW;
      }
    }
    Horizontal_Servo.write(H_current_position);
  }
  measure_voltage(25, 6.0);
}
// int Energy_Mangement(){

// }
//
void Energy_Management() {
  // Fan Controll
  Serial.println("Executing Energy Management");
  if (digitalRead(sensor) == HIGH) {
    digitalWrite(fan, HIGH);
    Serial.println("Truning on the FAN");
  } else {
    digitalWrite(fan, LOW); // turn relay OFF (light OFF)
    Serial.println("Turning off the FAN");
  }
  // Light Controll
  if (analogRead(ldr) >= 3600) {
    digitalWrite(led, LOW);
  } else {
    digitalWrite(led, HIGH);
  }
  //
}
void wifi_connect() {
  const char *ssid = "Test";
  const char *password = "12345678";

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}
bool Thinkspeak() {
  // Thinkspeak
  ThingSpeak.begin(client);
}
bool Thinkspeak_upload(short id, float sensor_value) {
  ThingSpeak.setField(id, sensor_value);

  int httpCode = ThingSpeak.writeFields(CHANNEL_ID, apiKey);

  if (httpCode == 200) {
    Serial.println("Sensor data sent to ThingSpeak!");
  } else {
    Serial.println("Error sending sensor data to ThingSpeak.");
  }

  delay(10);
}
float measure_voltage(short pin, int og_voltage) {
  return (analogRead(pin) / 4096) * 3.3;
}
