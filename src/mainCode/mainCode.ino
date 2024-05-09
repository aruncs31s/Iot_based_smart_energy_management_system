#include "configs/configs.h"
#include "configs/pins.h"
#include "configs/project.h"
#include <ESP32Servo.h>
#include <ThingSpeak.h>
#include <WiFi.h>
// #include <WiFiClient.h>

Servo Vertical_Servo;
Servo Horizontal_Servo;

ThingSpeakData Data;

const char *ssid = "TP-Link_2.4G";
const char *password = "AP32#0597ap@32";
const char *server = "api.thingspeak.com";
const char *apiKey = "TP84T1ANZST8RVXM";

void Solar_Tracking();
void Energy_Mangement();
void Energy_Monitoring();
float getVPP(int sensor);

// PIN Decelerations
const int &tol = TOLERANCE;
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
  pinMode(led, OUTPUT);   // initialize relay as an output
  pinMode(fan, OUTPUT);   // initialize relay as an output
  pinMode(sensor, INPUT); // initialize sensor as an input
                          //
  delay(3000);

  // NOTE: Configs related to Energy_Mangement

  pinMode(RADAR_PIN, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  ThingSpeak.begin(client);

  Serial.println("Finished Configuring");
  delay(500);
}
void loop() {
  // Solar_Tracking();
  Energy_Management();
  Energy_Monitoring();
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
  delay(1);
}

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
 
const int c_sensor = CURRENT_SENSOR_PIN;
const int mVperAmp =
    100; // this the 5A version of the ACS712 -use 100 for 20A Module
void Energy_Monitoring() { 
  float Watt = 0;
  double Voltage = 0;
  double VRMS = 0;
  double AmpsRMS = 0;
   Voltage = getVPP(c_sensor);
  VRMS = (Voltage / 2.0) * 0.707; // root 2 is 0.707
  AmpsRMS =
      ((VRMS * 1000) / mVperAmp) ; // 0.48 is the error when measuring the amp

  Serial.print(AmpsRMS);
  Serial.print(" Amps RMS  ---  ");
  Watt =  (AmpsRMS * 5.0 / 1.2);
  // note: 1.2 is my own empirically established calibration factor
  // as the voltage measured at D34 depends on the length of the OUT-to-D34 wire
  // 240 is the main AC power voltage – this parameter changes locally
  Serial.print(Watt);
  
  
  delay(1); 
  ThingSpeak.writeField(2537564, 2, Watt, apiKey);
}
float getVPP(int sensor) {
   float result;
  short readValue;       // value read from the sensor
  short maxValue = 0;    // store max value here
  short minValue = 4096; // store min value here ESP32 ADC resolution

  uint32_t start_time = millis();
  while ((millis() - start_time) < 1000) // sample for 1 Sec
  {
    readValue = analogRead(c_sensor);
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

  // Subtract min from max
  result = ((maxValue - minValue) * 3.3) / 4096.0; // ESP32 ADC resolution 4096
}
