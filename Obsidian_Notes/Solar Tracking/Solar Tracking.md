---
id: Solar Tracking
aliases: []
tags: []
---

## Solar Tracking
- [Introduction](Introduction)
- [Tasks](#tasks)
- [Todos](#todos)
- [Codes](#codes)
- 
#### Tasks
- [ ] Rotate The Solar Panel according to sunlight 
- [ ] Measure the output voltage using esp32
#### Todos 
- [ ] 3D print the model

#### Introduction
*Adjust the panel position according to available sunlight using Servo motor*

#working
[[LDR Sensor]] 
[[Servo Motor]]

#### Flow Charts
- [Flow Chart From Reference](Solar%20Trackng%20System.pdf#page=5)
*In this system, if the top LDR reading is higher than the down LDR reading, the servo motor will tilt the solar panel forward. Conversely, if the down LDR reading is higher than the top LDR reading, the servo motor will tilt the solar panel backward. Similarly, if the left LDR reading is greater than the right LDR reading, the servo motor will rotate clockwise. Conversely, if the right LDR reading is greater than the left LDR reading, the servo motor will rotate counterclockwise.*

$$if
$$
$$Top_{Reading} > Bottom_{Reading}$$
$$ Servo Motor = Tilt Forward$$
$$else$$
$$ ServoMotor = Tilt Backward$$
$$ if $$
$$ Right_{Reading} > Left_{Reading}$$
$$ ServoMotor = Rotate_{Clock Wise}$$
$$else$$
$$ ServoMotor = Rotate_{Anti Clockwise}$$


#### Codes

#TestCode
```c
#include <Servo.h>

Servo servohori; //horizontal servo(BOTTOM SERVO)
int servoh = 0; //assign servo at 0 degree
int servohLimitHigh = 180; //maximum range of servo is 180 degree(it is variable you can also change)
int servohLimitLow = 10;   //minimum range of servo is 10 degree(it is variable you can also change)

Servo servoverti; //vertical servo(TOP SERVO) 
int servov = 0; 
int servovLimitHigh = 180;
int servovLimitLow = 10;

int ldrtopr = 1; //top right LDR A1 pin
int ldrtopl = 2; //top left LDR A2 pin

int ldrbotr = 0; // bottom right LDR A0 pin
int ldrbotl = 3; // bottom left LDR A3 pin


 void setup () 
 {
  servohori.attach(10); //horizontal servo connected to arduino pin 10
  servohori.write(0);
  
  servoverti.attach(9); //vertical servo connected to arduino pin 9
  servoverti.write(0);
  delay(500); //delay
 }

void loop()
{
  servoh = servohori.read();
  servov = servoverti.read();
  
  
  int topl = analogRead(ldrtopl); //read analog values from top left LDR
  int topr = analogRead(ldrtopr); //read analog values from top right LDR
  int botl = analogRead(ldrbotl); //read analog values from bottom left LDR
  int botr = analogRead(ldrbotr); //read analog values from bottom right LDR
  
  
  int avgtop = (topl + topr) / 2; //average of top LDRs
  int avgbot = (botl + botr) / 2; //average of bottom LDRs
  int avgleft = (topl + botl) / 2; //average of left LDRs
  int avgright = (topr + botr) / 2; //average of right LDRs

  if (avgtop < avgbot)
  {
    servoverti.write(servov -1);
    if (servov > servovLimitHigh) 
     { 
      servov = servovLimitHigh;
     }
    delay(8);
  }
  else if (avgbot < avgtop)
  {
    servoverti.write(servov +1);
    if (servov < servovLimitLow)
  {
    servov = servovLimitLow;
  }
    delay(8);
  }
  else 
  {
    servoverti.write(servov);
  }
  
  if (avgleft > avgright)
  {
    servohori.write(servoh -1);
    if (servoh > servohLimitHigh)
    {
    servoh = servohLimitHigh;
    }
    delay(8);
  }
  else if (avgright > avgleft)
  {
    servohori.write(servoh +1);
    if (servoh < servohLimitLow)
     {
     servoh = servohLimitLow;
     }
    delay(8);
  }
  else 
  {
    servohori.write(servoh); // write means run servo
  }
  delay(50);
}

```

#WholeCode

```c
/*
Author : Arun CS
Github : https://github.com/aruncs31s
*/

#include <ESP32Servo.h>

// Define the GPIO (2,15) pin connected to the servo
const int SERVO_PIN_1 = 2; //   
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
```

#modifiedCode

```cpp
/*
Arun CS
Github : https://github.com/aruncs31s
Aswanth Mahesh
Github : https://github.com/AswanthMahesh
*/

#include <ESP32Servo.h>

// Define the GPIO (2,15) pin connected to the servo
const int SERVO_PIN_1 = 2; //   
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
const int SERVO_LIMIT_HIGH = 160;
const int SERVO_LIMIT_LOW = 20;

int current_Servo_1_value = 0;

// Create a Servo object
Servo Servo_1; // Horizontal
Servo Servo_2; // Vertical

void setup() {
  Serial.begin(9600);

  // Attach the servo to the GPIO pin
  Servo_1.attach(SERVO_PIN_1); // Horizontal
  Servo_2.attach(SERVO_PIN_2); // Vertical

  // Bring the servo to inital Position
  Servo_1.write(90); // Horizontal Servo 90
  Servo_2.write(90); // Vertical Servo to 90
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
```
#### References

- [[Solar Trackng System.pdf]]
- 
