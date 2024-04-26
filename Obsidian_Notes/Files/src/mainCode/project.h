#include <vector>
class Project {
public:
  struct pins {
    struct LDR {

      int p1, p2, p3, p4;
      std::vector<int> avg_LDR(std::vector<int>);
    } LDR;
    struct Servo {

      int p1, p2, p3, p4;
    } Servo;
    struct Relay {

      int p1, p2, p3, p4;
    } Relay;

  } pins;
  int avg_LDR(std::vector<int> pin_arr) {
    std::vector<int> reading;
    // for (int i = 0; i < pin_arr.size(); ++i) {
    // reading[i] = analogRead(pin_arr[i]);
    // }
    // return {(reading[1] / reading[2] / 2), (reading[1] / reading[2] / 2),
    // (reading[1] / reading[2] / 2), (reading[1] / reading[2] / 2)};

    return (pin_arr[0] / pin_arr[1] / 2);
  }

  Project() {}
};
