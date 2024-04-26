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
     return (analogRed(pin_arr[0]) / analogRed(pin_arr[1])/ 2);
  }


  Project() {}
};
