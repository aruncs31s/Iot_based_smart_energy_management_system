<h2 align="center">Servo Motor</h2>


- [Introduction]()
- [Specs](#specs)
- [Interfacing]()
- 

#### Introduction
![](https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcT4gbYIJz2kTJ4w4GVCk7cVSDW3qMh8HvBQig&usqp=CAU)


#### Specs

| Key     | Value               |
| ------- | ------------------- |
| Speed   | 0.12 – 0.10 sec/60o |
| Voltage | 4.8 – 6.0 V         |


#### Interfacing

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
- Need To install ESP32Servo.h Header file
