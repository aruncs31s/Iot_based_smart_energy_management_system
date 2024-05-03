#include <vector>
class Project_Config {
public:
  void pass_to_core(const char *&Func_Name, const char &Task_Name,
                    int &core_no) {};
  struct pins {
    struct LDR {

      short p1, p2, p3, p4;
      short avg_LDR(const std::vector<int> &pin_arr);
    } LDR;
    struct Servo {

      short p1, p2;
      void Servo_attach(int &pin) {}
    } Servo;
    struct Relay {

      short p1, p2;
    } Relay;

  } pins;
  int avg_LDR(const std::vector<int> &pin_arr) {
    Serial.println("Calculating AVG");
    Serial.print(pin_arr[0]); Serial.print("  and  "); Serial.print(pin_arr[1]);
    // return (analogRead(pin_arr[0]) + analogRead(pin_arr[1]) / 2);
  }
  Project_Config() {}
};
