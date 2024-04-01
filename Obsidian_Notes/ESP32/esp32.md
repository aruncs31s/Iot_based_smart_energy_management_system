---
id: esp32
aliases: []
tags: []
---


## ESP32

### Contents
- [Specs](#esp32%20specs)
- [[Pinout.canvas|Pinout]]
- [ADC Interfacing](/ESP32/analog_interfacing)
* Interfacing
	- [LDR Interfacing](#ldr%20interfacing)
	- [PIR Interfaing](#pir%20interfacing)
	- [Servo Motor Interfacing](#servo%20motor%20interfacing)
- Whole Code
	- [Servo Motor]()
- [Interfaced Pins](#interfaced%20pins)
- [Multi Threding](#multi%20threding)
- 
##### Esp32 Specs

- Single or Dual-Core ==32-bit== LX6 Microprocessor with clock frequency up to ==240 MHz==.
- 520 KB of SRAM, 448 KB of ROM and 16 KB of RTC SRAM.
- Supports 802.11 b/g/n Wi-Fi connectivity with speeds up to 150 Mbps.
- Support for both Classic Bluetooth v4.2 and ==BLE== specifications.
- ==34== Programmable GPIOs.
- Up to 18 channels of ==12-bit SAR ADC== and 2 channels of ==8-bit DAC==
- Serial Connectivity include 4 x ==SPI==, 2 x ==I2C==, 2 x ==I2S==, 3 x ==UART==.
- Ethernet MAC for physical LAN Communication (requires external PHY).
- 1 Host controller for SD/SDIO/MMC and 1 Slave controller for SDIO/SPI.
- Motor PWM and up to 16-channels of LED PWM.
- Secure Boot and Flash Encryption.
- Cryptographic Hardware Acceleration for AES, Hash (SHA-2), RSA, ECC and RNG

###### More
- [[ESP32_Specification.pdf]]
- [[esp32-wroom-32_datasheet_en.pdf]]
- [[Cadence_Tensillica_Xtensa_LX6_ds.pdf]]
- 




#### LDR Interfacing

```c
const int LDR_PIN = 36;

void setup() {
   Serial.begin(9600);
   analogReadResolution(10); //default is 12. Can be set between 9-12.
}
void loop() {
   float LDR_Voltage = ((float)LDR_Reading*3.3/1023);
   Serial.print("Reading: ");Serial.print(LDR_Reading); Serial.print("\t");Serial.print("Voltage: ");Serial.println(LDR_Voltage);
}

```


#### PIR Interfacing
[Source](https://www.electronicwings.com/esp32/pir-sensor-interfacing-with-esp32)
```c
  Serial.println("Waiting For Power On Warm Up");
  delay(20000); /* Power On Warm Up Delay */
  Serial.println("Ready!");
}
int sensor_output;
void loop() {
  
  sensor_output = digitalRead(PIR_SENSOR_OUTPUT_PIN);
  Serial.println(sensor_output + "\n"); 
  delay(100);
  // if( sensor_output == LOW )
  // {
  //   if( warm_up == 1 )
  //    {
  //     Serial.print("Warming Up\n\n");
  //     warm_up = 0;
  //     delay(2000);
  //   }
  //   // Serial.print("No object in sight\n\n");
  //   delay(1000);
  // }
  // else
  // {
  //   // Serial.print("Object detected\n\n");   
  //   warm_up = 1;
  //   delay(100);
  // } 
}
```

---

#### Servo Motor Interfacing

```c
/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/
  Written by BARRAGAN and modified by Scott Fitzgerald
*********/

#include <Servo.h>

static const int servoPin = 13;

Servo servo1;

void setup() {
  Serial.begin(115200);
  servo1.attach(servoPin);
}

void loop() {
  for(int posDegrees = 0; posDegrees <= 180; posDegrees++) {
    servo1.write(posDegrees);
    Serial.println(posDegrees);
    delay(20);
  }

```


#### Servo Motor Interfacing 2

#TestCode
[Source](https://chat.openai.com/)

```c
#include <ESP32Servo.h>

// Define the GPIO pin connected to the servo
const int servoPin = 2;

// Create a Servo object
Servo myservo;

void setup() {
  Serial.begin(9600);

  // Attach the servo to the GPIO pin
  myservo.attach(servoPin);
}

void loop() {
  // Sweep the servo back and forth
  for (int angle = 0; angle <= 180; angle += 1) {
    myservo.write(angle);
    delay(15);
  }
  for (int angle = 180; angle >= 0; angle -= 1) {
    myservo.write(angle);
    delay(15);
  }
}
```

#### Interfaced Pins

#PinConfiguration

| Pin No | GPIO   | Used For                                        |     |
| ------ | ------ | ----------------------------------------------- | --- |
| D2     | GPIO2  | Connecting the Horzontal Servo - Solar Tracking |     |
| D15    | GPIO15 | Connecting the Vertical Servo - Solar Tracking  |     |
| D34    | GPIO34 | Connecting the LDR at Top Right                 |     |
| D35    | GPIO35 | Connecting the LDR at Bottom Right              |     |
| VP     | GPIO36 | Connecting the LDR at Top Left                  |     |
| VN     | GPIO39 | Connecting the LDR at Top Right                 |     |
| D13    | GPIO13 | Connecting the PIR Sensor                       |     |
#pinDefine
```c
// Define the GPIO (2,15) pin connected to the servo
const int SERVO_PIN_1 = 2;
const int SERVO_PIN_2 = 15;
// Define the GPIO ADC Pins for LDR Sensors
const int LDR_LEFT_TOP = 36;
const int LDR_LEFT_BOTTOM = 39;
const int LDR_RIGHT_TOP = 34;
const int LDR_RIGHT_BOTTOM = 35;
```

#### Multi Threding
```c
/*
Source : https://randomnerdtutorials.com/esp32-dual-core-arduino-ide/ */
*/
/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

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

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Solar_Managment,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &solar_tracking_id,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Energy_Managment,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &energy_managment_id,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
}

//Task1code: blinks an LED every 1000 ms
void Solar_Managment( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    digitalWrite(led1, HIGH);
    delay(1000);
    digitalWrite(led1, LOW);
    delay(1000);
  } 
}

//Task2code: blinks an LED every 700 ms
void Energy_Managment( void * pvParameters ){
  
  
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  // for(;;){
  //   digitalWrite(led2, HIGH);
  //   delay(700);
  //   digitalWrite(led2, LOW);
  //   delay(700);
  // }
}

void loop() {
  
}


```
