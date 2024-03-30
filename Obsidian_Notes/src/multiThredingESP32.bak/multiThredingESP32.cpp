
/*
Edited By : Arun CS
Gtihub : https://github.com/aruncs31s/
*/
/* Thanks To
 https://randomnerdtutorials.com/esp32-dual-core-arduino-ide/
 Rui Santos - https://randomnerdtutorials.com
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

TaskHandle_t solar_tracking_id;
TaskHandle_t energy_managment_id;

// LED pins
const int led1 = 2;
const int led2 = 4;
//
void setup() {
  Serial.begin(9600);
  // pinMode(led1, OUTPUT);
  // pinMode(led2, OUTPUT);
  // Attach the servo to the GPIO pin
  Servo_1.attach(SERVO_PIN_1);
  Servo_2.attach(SERVO_PIN_2);

  // Bring the servo to inital Position
  Servo_1.write(0);
  Servo_2.write(0);
  delay(500); // Need To find Out :)

  // Read The LDR Values
  // create a task that will be executed in the Task1code() function, with
  // priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
      Solar_Managment,    /* Task function. */
      "Task1",            /* name of task. */
      10000,              /* Stack size of task */
      NULL,               /* parameter of the task */
      1,                  /* priority of the task */
      &solar_tracking_id, /* Task handle to keep track of created task */
      0);                 /* pin task to core 0 */
  delay(500);

  // create a task that will be executed in the Task2code() function, with
  // priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
      Energy_Managment,     /* Task function. */
      "Task2",              /* name of task. */
      10000,                /* Stack size of task */
      NULL,                 /* parameter of the task */
      1,                    /* priority of the task */
      &energy_managment_id, /* Task handle to keep track of created task */
      1);                   /* pin task to core 1 */
  delay(500);
}

// Task1code: blinks an LED every 1000 ms
void Solar_Managment(void *pvParameters) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  //
  // for(;;){
  //   digitalWrite(led1, HIGH);
  //   delay(1000);
  //   digitalWrite(led1, LOW);
  //   delay(1000);
  // }
  //
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

// Task2code: blinks an LED every 700 ms
void Energy_Managment(void *pvParameters) {
  /* Todo
  - [ ] Add a method PIR Sensor interfacing to turn on the fan = contorl the
  relay module
  - [ ] Add a method to turn of light ass soon as the outside light is more than
  our fixed threshold = 1023 = V_ref
  - [ ]
    */

  const int PIR_SENSOR_PIN = 13;
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    digitalWrite(led2, HIGH);
    delay(700);
    digitalWrite(led2, LOW);
    delay(700);
  }
}

void loop() {}
