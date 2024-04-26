
/*
Author : Arun CS
Gtihub : https://github.com/aruncs31s/
*/

#include <ESP32Servo.h>

// NOTE: All Pin Declaration Must be done here

// HACK: make struct so to use Config.module().pins.getPIN  => which returns an

class Project {
  struct pins {
    const int pin1, pin2, pin3, pin4, pin5, pin6, pin7;

    // Constructor to initialize the pins
    pins(int p1, int p2, int p3, int p4)
        : pin1(p1), pin2(p2), pin3(p3), pin4(p4) {}
  };
  struct digital_pins {
    const int pin1, pin2, pin3;

    // Constructor to initialize the digital pins
    digital_pins(int p1, int p2, int p3) : pin1(p1), pin2(p2), pin3(p3) {}
  } digital;

  struct analog_pins {
    const int pin1, pin2, pin3;

    // Constructor to initialize the analog pins
    analog_pins(int p1, int p2, int p3) : pin1(p1), pin2(p2), pin3(p3) {}
  } analog;

  struct LDR_pins {
    const int pin1, pin2, pin3, pin4;

    // Constructor to initialize the LDR pins
    LDR_pins(int p1, int p2, int p3, int p4)
        : pin1(p1), pin2(p2), pin3(p3), pin4(p4) {}
  } LDR;
};

Project SolarTracking;
Project EnergyManagment;

// TODO:  Rearange the pins or rearange the structure where the ldr is placed to
// match the reading values and position

// Solar Tracking Pin Declarations
SolarTracking.LDR = Project::LDR_pins();

int LDR_LEFT_TOP = SolarTracking.LDR.pin1,
    LDR_LEFT_BOTTOM = SolarTracking.LDR.pin2,
    LDR_RIGHT_TOP = SolarTracking.LDR.pin3,
    LDR_RIGHT_BOTTOM = SolarTracking.LDR.pin4;

const int RELAY_PIN_1 = 26, RELAY_PIN_2 = 27, PIR_SENSOR_PIN = 13;

// Interfaced Pins for  Solar Tracking
// Define the GPIO (2,15) pin connected to the servo for Solar tracking
// FIXME: Find which servo(Horizontal,Vertical) is connected to which pin
const int SERVO_PIN_1 = 25, // For connecting the horizontal servo
    SERVO_PIN_2 = 33;       // For connecting the Vertical servo

// Define the GPIO ADC Pins for LDR Sensors

// Setting the LDR resistance Value thresholds
const int LDR_VALUE_HIGH = 900, LDR_VALUE_LOW = 20;

// Servo Motor Rotation Values
const int SERVO_LIMIT_HIGH = 170, SERVO_LIMIT_LOW = 10;

int current_Servo_1_value = 0;

// Create a Servo object
Servo Servo_1; // Horizontal
Servo Servo_2; // Vertical

TaskHandle_t solar_tracking_id;
TaskHandle_t energy_management_id;

// LED pins
const int led1 = 2;
const int led2 = 4;
//

// TODO: Figure out Blynk or Thinkspeak interfacing code

void setup() {
  Serial.begin(9600);

  // Set ADC value as 10 bit
  (int)analogReadResolution(10);

  // Attach the servo to the GPIO pin
  Servo_1.attach(SERVO_PIN_1);
  Servo_2.attach(SERVO_PIN_2);

  // Bring the servo to initial Position
  Servo_1.write(0);
  Servo_2.write(0);

  delay(500);

  // Read The LDR Values
  // create a task that will be executed in the Task1code() function, with
  // priority 1 and executed on core 0
  // xTaskCreatePinnedToCore(
  //   Solar_Management,   /* Task function. */
  //   "Task1",     /* name of task. */
  //   10000,       /* Stack size of task */
  //   NULL,        /* parameter of the task */
  //   1,           /* priority of the task */
  //   &solar_tracking_id,      /* Task handle to keep track of created task */
  //   0);          /* pin task to core 0 */
  // delay(500);

  // FIXME: After Test uncomment + 11
  // create a task that will be executed in the Task2code() function, with
  // priority 1 and executed on core 1
  //   xTaskCreatePinnedToCore(
  //     Energy_Management,   /* Task function. */
  //     "Task2",     /* name of task. */
  //     10000,       /* Stack size of task */
  //     NULL,        /* parameter of the task */
  //     1,           /* priority of the task */
  //     &energy_management_id,      /* Task handle to keep track of created
  //     task */ 1);          /* pin task to core 1 */
  //   delay(500);
  //
}

// Task1code: blinks an LED every 1000 ms

// Task2code: blinks an LED every 700 ms

void loop() {
  // OPTIMIZE: Need to optimize this code
  int reading_LDR_left_avg, reading_LDR_right_avg, reading_LDR_bottom_avg,
      reading_LDR_top_avg;
  // Serial.print("Task1 running on core ");
  // Serial.println(xPortGetCoreID());
  // (int)analogRead(LDR_LEFT_BOTTOM) = analogRead(LDR_LEFT_BOTTOM);
  // (int)analogRead(LDR_LEFT_TOP) = analogRead(LDR_LEFT_TOP);
  // (int)analogRead(LDR_RIGHT_TOP) = analogRead(LDR_RIGHT_TOP);
  // (int)analogRead(LDR_RIGHT_BOTTOM) = analogRead(LDR_RIGHT_BOTTOM);

  reading_LDR_left_avg =
      ((int)analogRead(LDR_LEFT_TOP) + (int)analogRead(LDR_LEFT_BOTTOM)) / 2;
  reading_LDR_right_avg =
      ((int)analogRead(LDR_RIGHT_BOTTOM) + (int)analogRead(LDR_RIGHT_TOP)) / 2;
  reading_LDR_bottom_avg =
      ((int)analogRead(LDR_RIGHT_BOTTOM) + (int)analogRead(LDR_LEFT_BOTTOM)) /
      2;
  reading_LDR_top_avg =
      ((int)analogRead(LDR_RIGHT_TOP) + (int)analogRead(LDR_LEFT_TOP)) / 2;

  // FIXME:
  /*
   while (1){
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
}
        */
  delay(3000);
  Serial.print("bottom left :  ");
  Serial.println((int)analogRead(LDR_LEFT_BOTTOM));
  Serial.print("top left :  ");
  Serial.println((int)analogRead(LDR_LEFT_TOP));
  Serial.print("top right :  ");
  Serial.println((int)analogRead(LDR_RIGHT_TOP));
  Serial.print("bottom right :   ");
  Serial.println((int)analogRead(LDR_RIGHT_BOTTOM));

  // Serial.print("bottom avg :  ");
  // Serial.println(reading_LDR_bottom_avg);
  // Serial.print("top avg :  ");
  // Serial.println(reading_LDR_top_avg);
  // Serial.print("left avg :  ");
  // Serial.println( reading_LDR_left_avg);
  // Serial.print("right avg :   ");
  // Serial.println( reading_LDR_right_avg);
}
void Energy_Management(void *pvParameters) {

  /* Todo
  - [ ] Add a method PIR Sensor interfacing to turn on the fan = control the
  relay module
  - [ ] Add a method to turn of light ass soon as the outside light is more than
  our fixed threshold = 1023 = V_ref
  - [ ]
    */

  pinMode(PIR_SENSOR_PIN, INPUT);

  int warm_up;
  int sensor_output = digitalRead(PIR_SENSOR_PIN);

  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  while (1) {
    if (sensor_output == LOW) {
      if (warm_up == 1) {
        Serial.print("Warming Up\n\n");
        warm_up = 0;
        delay(2000);
      }
      Serial.print("No object in sight\n\n");
      delay(1000);
    } else {
      Serial.print("Object detected\n\n");
      warm_up = 1;
      delay(1000);
    }
  }
}

void Solar_Management(void *pvParameters) {

  project SolarTracking;

  // OPTIMIZE: Need to optimize this code
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  int(int) analogRead(LDR_LEFT_BOTTOM) = analogRead(LDR_LEFT_BOTTOM);
  int(int) analogRead(LDR_LEFT_TOP) = analogRead(LDR_RIGHT_TOP);
  int(int) analogRead(LDR_RIGHT_TOP) = analogRead(LDR_RIGHT_TOP);
  int(int) analogRead(LDR_RIGHT_BOTTOM) = analogRead(LDR_RIGHT_BOTTOM);

  int reading_LDR_left_avg =
      ((int)analogRead(LDR_LEFT_TOP) + analogRead(LDR_LEFT_BOTTOM)) / 2;
  int reading_LDR_right_avg =
      ((int)analogRead(LDR_RIGHT_BOTTOM) + analogRead(LDR_RIGHT_TOP)) / 2;
  int reading_LDR_bottom_avg =
      ((int)analogRead(LDR_RIGHT_BOTTOM) + analogRead(LDR_LEFT_BOTTOM)) / 2;
  int reading_LDR_top_avg =
      ((int)analogRead(LDR_RIGHT_TOP) + analogRead(LDR_LEFT_TOP)) / 2;

  // FIXME:
  /*
   while (1){
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
}
        */
  delay(50);
  Serial.print("bottom left :  ");
  Serial.println((int)analogRead(LDR_LEFT_BOTTOM));
  Serial.print("top left :  ");
  Serial.println((int)analogRead(LDR_LEFT_TOP));
  Serial.print("top right :  ");
  Serial.println((int)analogRead(LDR_RIGHT_TOP));
  Serial.print("bottom right :   ");
  Serial.println((int)analogRead(LDR_RIGHT_BOTTOM));
}
