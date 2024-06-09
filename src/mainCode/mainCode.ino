#include "configs/configs.h"
#include "configs/pins.h"
#include "configs/project.h"
#include <ESP32Servo.h>
#include <ThingSpeak.h>
#include <WiFi.h>
// #include <WiFiClient.h>

Servo Vertical_Servo;
Servo Horizontal_Servo;

const char *ssid = "miniproject";
const char *password = "12345678";
const char *server = "api.thingspeak.com";
const char *apiKey = "TP84T1ANZST8RVXM";

// Global Variable Decleration To be Acceses by all the functions
int solar_panel_voltage = 0;
int fan_power_consumption = 0;
short V_current_position = 45;
short H_current_position = 180;
bool fan_status;
bool led_status;

void Solar_Tracking();
void Energy_Mangement();
void Energy_Monitoring();
float getVPP();

// TaskHandle_t solar_tracking_id;
// TaskHandle_t energy_managment_id;
// TaskHandle_t thingspeak_id;
//
// PIN Decelerations

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
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  delay(400);
  ThingSpeak.begin(client);
  Vertical_Servo.write(V_current_position);
  Horizontal_Servo.write(H_current_position);

  Serial.println("Finished Configuring");
  // xTaskCreatePinnedToCore(ThingSpeak_Upload, "ThingSpek", 10000, NULL, 1,
  //                         &thingspeak_id, 0);
  delay(500);
}
void loop() {
  Serial.println("Executing Solar Tracking");
  Solar_Tracking();
  Serial.println("Executing Energy_Management");
  Energy_Management();
  // Serial.println("Executing Energy_Monitoring");
  // Energy_Monitoring();
  delay(10);
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
    Serial.println("avg left : " + String(avg_left));
    Serial.println("avg roght " + String(avg_right));
    int x = ThingSpeak.writeField(2537564, 4, V_current_position, apiKey);
    delay(50);
    int y = ThingSpeak.writeField(2537564, 3, H_current_position, apiKey);
    delay(50);
  }
  delay(10);
}

void Energy_Management() {
  // Fan Controll
  Serial.println("Executing Energy Management");
  Serial.println("Radar Sensor Value : " + String(digitalRead(RADAR_PIN)));
  if (digitalRead(RADAR_PIN) == HIGH) {
    digitalWrite(RELAY_PIN_FAN, HIGH);
    Serial.println("Turning on the FAN");
    fan_status = 1;

  } else {
    digitalWrite(RELAY_PIN_FAN, LOW); // turn relay OFF (light OFF)
    Serial.println("Turning off the FAN");
    fan_status = 0;
  }
  // Light Controll
  Serial.println("LDR Value of EM : " + String(analogRead(LDR_PIN)));

  if (analogRead(LDR_PIN) >= 3600) {
    digitalWrite(RELAY_PIN_LED, LOW);
    Serial.println("Light OFF");
    led_status = 0;
  } else {
    digitalWrite(RELAY_PIN_LED, HIGH);
    Serial.println("Light ON");
    led_status = 1;
  }
  String s_fan_status = fan_status ? "true" : "false";
  String s_led_status = led_status ? "true" : "false";
  int x = ThingSpeak.writeField(2537564, 5, s_fan_status, apiKey);
  delay(20);
  int y = ThingSpeak.writeField(2537564, 6, s_led_status, apiKey);
  delay(20);
  //
}
