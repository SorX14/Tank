time_t getTeensy3Time() {
  return Teensy3Clock.get(); 
}

void printDigits(int digits) {
  if (digits < 10) {
    display.print("0");
  }
  display.print(digits);
}

bool frameAction(int divider) {
  static int count = 0;

  if (count == (divider * 2)) {
    count = 0;
  }

  if (count < divider) {
    count++;
    return true;
  }

  count++;

  return false;	
}

void resetI2CDevices() {
  // - Remove the interrupt handler
  // - Change pin to output
  // - Set it low (resetting all devices)
  // - Set to high
  // - Reattach the interrupt handler

  detachInterrupt(RESET);
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, LOW);
  delay(100);
  digitalWrite(RESET, HIGH);
  pinMode(RESET, INPUT);
  attachInterrupt(RESET, resetTeensy, FALLING);
}

