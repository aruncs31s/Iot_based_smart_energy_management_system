#define LDR_TOP_RIGHT 34
#define LDR_TOP_LEFT 35
#define LDR_BOTTOM_RIGHT 36
#define LDR_BOTTOM_LEFT 39

#define HORIZONTAL_SERVO 2
#define VERTICAL_SERVO 15
#define SERVO_PIN3 7
#define SERVO_PIN4 8

#define RELAY_PIN1 9
#define RELAY_PIN2 10
#define RELAY_PIN3 11
#define RELAY_PIN4 12

// #include "configs.h"
#include "project.h"
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
Project SolarTracking;
Project EnergyManagment;
Project EnergyMonitoring;

int main() {
  // void setup() {

  // const int &x = HORIZONTAL_SERVO;
  // std::cout << avg[1] << "";
  // for (int i = 0; i < avg.size(); ++i) {
  //   std::cout << avg[i] << "\n";
  // }
  // int avg_left,avg[0];;
  // int avg_right = avg[1];
  // cout << SolarTracking.pins.LDR.p1;
}

// void loop() {}
//
// int main() {
// setup();
// main();
// }
//
int SolarTracking() {

  while (1) {
    SolarTracking.pins.LDR = {.p1 = LDR_TOP_RIGHT,
                              .p2 = LDR_TOP_LEFT,
                              .p3 = LDR_BOTTOM_RIGHT,
                              .p4 = LDR_BOTTOM_LEFT};
    SolarTracking.pins.Servo = {
        .p1 = HORIZONTAL_SERVO,
        .p2 = VERTICAL_SERVO,
    };

    int avg_top = SolarTracking.avg_LDR(
        {SolarTracking.pins.LDR.p1, SolarTracking.pins.LDR.p2});
    int avg_bottom = SolarTracking.avg_LDR(
        {SolarTracking.pins.LDR.p3, SolarTracking.pins.LDR.p4});
    int avg_right = SolarTracking.avg_LDR(
        {SolarTracking.pins.LDR.p1, SolarTracking.pins.LDR.p3});
    int avg_left = SolarTracking.avg_LDR(
        {SolarTracking.pins.LDR.p2, SolarTracking.pins.LDR.p4});

    // const int &x = HORIZONTAL_SERVO;
    // std::cout << avg[1] << "";
    // for (int i = 0; i < avg.size(); ++i) {
    //   std::cout << avg[i] << "\n";
    // }
    // int avg_left,avg[0];;
    // int avg_right = avg[1];
    // cout << SolarTracking.pins.LDR.p1;
  }
}
