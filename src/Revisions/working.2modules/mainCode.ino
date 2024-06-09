#include "configs/configs.h"
#include "configs/pins.h"
#include "configs/project.h"
#include <ESP32Servo.h>
#include <ThingSpeak.h>
#include <WiFi.h>
// #include <WiFiClient.h>

Servo Vertical_Servo;
Servo Horizontal_Servo;

const char *ssid = "TP-Link_2.4G";
const char *password = "AP32#0597ap@32";
const char *server = "api.thingspeak.com";
const char *apiKey = "TP84T1ANZST8RVXM";

void Solar_Tracking();
void Energy_Mangement();
void Energy_Monitoring();
float getVPP(int sensor);

// PIN Decelerations
short V_current_position = 45;
short H_current_position = 180;

// // NOTE: Multithreading Handlers
//
// TaskHandle_t solar_tracking_id;
// TaskHandle_t energy_management_id;

// TODO: Complete the code Thinkspeak

WiFiClient client;

void setup() {
  Serial.begin(9600);
  // analogReadResolution(10);
  delay(50);

  Vertical_Servo.attach(VERTICAL_SERVO_PIN);
  Horizontal_Servo.attach(HORIZONTAL_SERVO_PIN);

  // For Energy_Mangement
  pinMode(RELAY_PIN_LED, OUTPUT); // initialize relay as an output
  pinMode(RELAY_PIN_FAN, OUTPUT); // initialize relay as an output
  pinMode(RADAR_PIN, INPUT);      // initialize sensor as an input
                                  //
  delay(3000);

  // NOTE: Configs related to Energy_Mangement

  pinMode(RADAR_PIN, INPUT);

  WiFi.begin(ssid, password);
  delay(400);
  ThingSpeak.begin(client);
  Vertical_Servo.write(V_current_position);
  Horizontal_Servo.write(H_current_position);

  Serial.println("Finished Configuring");
  delay(500);
}
void loop() {
  Serial.println("Executing Solar Tracking");
  Solar_Tracking();
  Serial.println("Executing Energy_Management");
  Energy_Management();
  // Serial.println("Executing Energy_Monitoring");
  // Energy_Monitoring();
}

void Solar_Tracking() {

  int avg_top = avg(LDR_TOP_RIGHT, LDR_TOP_LEFT);
  int avg_bottom = avg(LDR_BOTTOM_LEFT, LDR_BOTTOM_RIGHT);

  int avg_left = avg(LDR_TOP_LEFT, LDR_BOTTOM_LEFT);
  int avg_right = avg(LDR_TOP_RIGHT, LDR_BOTTOM_RIGHT);
  Serial.println("Top Left " + String(analogRead(LDR_TOP_LEFT)));
  Serial.println("BOttom Left" + String((analogRead(LDR_BOTTOM_LEFT))));
  Serial.println("Top Right " + String(analogRead(LDR_TOP_RIGHT)));
  Serial.println("Bottom Right" + String(analogRead(LDR_BOTTOM_RIGHT)));

  int diff_vert = avg_top - avg_bottom; // check the difference of up and down
  int diff_horiz =
      avg_left - avg_right; // check the difference of left and right
  if (-1 * TOLERANCE > diff_vert || diff_vert > TOLERANCE) {
    if (avg_top < avg_bottom) {
      V_current_position++;
      if (V_current_position > V_SERVO_LIMIT_HIGH) {
        V_current_position = V_SERVO_LIMIT_HIGH;
      }
    } else if (avg_top > avg_bottom) {
      V_current_position--;
      if (V_current_position < V_SERVO_LIMIT_LOW) {
        V_current_position = V_SERVO_LIMIT_LOW;
      }
    }
    Serial.print("V : ");
    Serial.println(V_current_position);
    Vertical_Servo.write(V_current_position);
  }

  if (-1 * TOLERANCE > diff_horiz || diff_horiz > TOLERANCE) {
    if (avg_left < avg_right) {
      H_current_position++;
      if (H_current_position > H_SERVO_LIMIT_HIGH) {
        H_current_position = H_SERVO_LIMIT_HIGH;
      }
    }
    if (avg_left > avg_right) {
      H_current_position--;
      if (H_current_position < H_SERVO_LIMIT_LOW) {
        H_current_position = H_SERVO_LIMIT_LOW;
      }
    }
    Horizontal_Servo.write(H_current_position);
    Serial.print("H : ");
    Serial.println(H_current_position);
    Serial.println("avg left : " + String(avg_left));
    Serial.println("avg roght " + String(avg_right));
  }
  delay(10);
}

void Energy_Management() {
  // Fan Controll
  Serial.println("Executing Energy Management");
  Serial.println("Radar Sensor Value : " + String(digitalRead(RADAR_PIN)));
  if (digitalRead(RADAR_PIN) == HIGH) {
    digitalWrite(RELAY_PIN_FAN, HIGH);
    Serial.println("Truning on the FAN");
  } else {
    digitalWrite(RELAY_PIN_FAN, LOW); // turn relay OFF (light OFF)
    Serial.println("Turning off the FAN");
  }
  // Light Controll
  Serial.println("LDR Value of EM : " + String(analogRead(LDR_PIN)));

  if (analogRead(LDR_PIN) >= 3600) {
    digitalWrite(RELAY_PIN_LED, LOW);
    Serial.println("Light OFF");
  } else {
    digitalWrite(RELAY_PIN_LED, HIGH);
    Serial.println("Light ON");
  }
  //
  delay(1);
}
void Energy_Monitoring() {
  float Watt = 0.0;
  float readValue;
  int maxValue = 4095;
  int minValue = 0;
  uint32_t start_time = millis();
  Serial.println(start_time);
  while (1000) // sample for 1 Sec
  {
    readValue = analogRead(2);
    // see if you have a new maxValue
    if (readValue > maxValue) {
      /*record the maximum sensor value*/
      maxValue = readValue;
    }
    if (readValue < minValue) {
      /*record the minimum sensor value*/
      minValue = readValue;
    }
  }
  float voltage = (readValue / 4096);

  Serial.println(voltage);
  delay(100);
  Serial.println(analogRead(2));

  ThingSpeak.writeField(2537564, 2, Watt, apiKey);
}
