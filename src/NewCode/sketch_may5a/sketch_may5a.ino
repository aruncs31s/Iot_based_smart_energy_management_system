#include <ESP32Servo.h>
//main1 og
const int relayLight = 21; // define relay pin for light
const int relayFan = 18; // define relay pin for fan
const int sensor = 15; // define sensor pin

Servo horizontal; // horizontal servo
int servoh = 180;
int servohLimitHigh = 175;
int servohLimitLow = 5;

Servo vertical; // vertical servo
int servov = 45;
int servovLimitHigh = 60;
int servovLimitLow = 30;

// LDR pin connections
// name = analogpin;
int ldrlt = 34; //LDR top left - BOTTOM LEFT <--- BDG
int ldrrt = 35; //LDR top right - BOTTOM RIGHT
int ldrld = 32; //LDR down left - TOP LEFT
int ldrrd = 33; //LDR down right - TOP RIGHT

void setup() {
  horizontal.attach(12); // Assuming GPIO18 for horizontal servo
  vertical.attach(13); // Assuming GPIO19 for vertical servo
  horizontal.write(180);
  vertical.write(45);
  delay(2500);

  Serial.begin(115200);
    pinMode(relayLight, OUTPUT); // initialize relay for light as an output
  pinMode(relayFan, OUTPUT); // initialize relay for fan as an output
  pinMode(sensor, INPUT); // initialize sensor as an input
  Serial.begin(115200); // initialize serial

}

void loop() {
  int lt = analogRead(ldrlt); // top left
  int rt = analogRead(ldrrt); // top right
  int ld = analogRead(ldrld); // down left
  int rd = analogRead(ldrrd); // down right
  int dtime = 10; 
  int tol = 200; // tolerance
  int avt = (lt + rt) / 2; // average value top
  int avd = (ld + rd) / 2; // average value down
  int avl = (lt + ld) / 2; // average value left
  int avr = (rt + rd) / 2; // average value right

  Serial.print("Average top: ");
  Serial.println(avt);
  Serial.print("Average bottom: ");
  Serial.println(avd);
  Serial.print("Average left: ");
  Serial.println(avl);
  Serial.print("Average right: ");
  Serial.println(avr);
  Serial.println();

  int dvert = avt - avd; // check the difference of up and down
  int dhoriz = avl - avr; // check the difference of left and right

  if (-1 * tol > dvert || dvert > tol) {
    if (avt < avd) {
      servov++;
      if (servov > servovLimitHigh) {
        servov = servovLimitHigh;
      }
    } else if (avt > avd) {
      servov--;
      if (servov < servovLimitLow) {
        servov = servovLimitLow;
      }
    }
    Serial.print("Current vertical Value: ");
    Serial.println(servov);
    Serial.print("Current horizontal Value: ");
    Serial.println(servoh);
    Serial.println();
    vertical.write(servov);
  }
  if (-1 * tol > dhoriz || dhoriz > tol) {
    if (avl < avr) {
      servoh++;
      if (servoh > servohLimitHigh) {
        servoh = servohLimitHigh;
      }
      
    } else if (avl > avr) {
      servoh--;
      if (servoh < servohLimitLow) {
        servoh = servohLimitLow;
      }
    }
    Serial.print("Current horizontal Value: ");
    Serial.println(servoh);

    Serial.print("Current vertical Value: ");
    Serial.println(servov);
    Serial.println();
    horizontal.write(servoh);
  }

  delay(dtime);


  int val = digitalRead(sensor); // read sensor value
  if (val == HIGH) { // check if the sensor is HIGH (motion detected)
    digitalWrite(relayLight, HIGH); // turn relay for light OFF
    digitalWrite(relayFan, HIGH); // turn relay for fan OFF
    Serial.println("Motion detected! Light and fan turned ON");
  } else {
    digitalWrite(relayLight, LOW); // turn relay for light ON
    digitalWrite(relayFan, LOW); // turn relay for fan ON
    Serial.println("No motion detected. Light and fan turned OFF");
  } 
  delay(100); // adjust the delay time according to your needs
}