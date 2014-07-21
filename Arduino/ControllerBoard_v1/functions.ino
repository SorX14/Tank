time_t getTeensy3Time() {
  return Teensy3Clock.get(); 
}

void printDigits(int digits) {
  if (digits < 10) {
    display.print("0");
  }
  display.print(digits);
}

