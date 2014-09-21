static void setLEDs() {
  // set to bits 30..23, but rounded by one extra bit (i.e. bit 22)
  int r = (byte) (((word) (now[0] >> 22) + 1) >> 1);
  int g = (byte) (((word) (now[1] >> 22) + 1) >> 1);
  int b = (byte) (((word) (now[2] >> 22) + 1) >> 1);
  
  analogWrite(RLED, r);
  analogWrite(GLED, g);
  analogWrite(BLED, b);
}

static void useRamp (const void* ptr) {
  const Ramp* ramp = (const Ramp*) ptr;
  
  nextRamp = ramp->chain;
  duration = ramp->steps * 10;
  for (byte i = 0; i < 3; ++i) {
    long target = (long) ramp->colors[i] << 23;
    if (duration > 0) {
      delta[i] = (target - now[i]) / duration;
    } else {
      now[i] = target;
    }
  }
  setLEDs();
}

static void loadRamp (byte pos) {
  Ramp ramp;
  ramp = stdRamps[pos];
  useRamp(&ramp);
}
