
const int LDR_TOP_LEFT = xx;
const int LDR_TOP_RIGHT = xx;
const int LDR_BOTOOM_LEFT = xx;
const int LDR_BOTTOM_RIGHT = xx;

void setup() {}

void loop() {

  short Hori_current_position = 0, Vert_current_position = 0;

  while (true) {
    if (avg(LDR_TOP_RIGHT, LDR_BOTTOM_RIGHT), )
  }
}

int avg(int pin_1, int pin_2) {
  return (analogRead(pin_1) + analogRead(pin_2)) / 2;
}
