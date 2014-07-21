// The interrupt handlers for all the RC pins
void RCchannel1 () {
  if (digitalRead(RC1PIN)) {
    channel1_start = micros();
  } else {
    channel1 = (uint16_t) (micros() - channel1_start);
  }
}

void RCchannel2 () {
  if (digitalRead(RC2PIN)) {
    channel2_start = micros();
  } else {
    channel2 = (uint16_t) (micros() - channel2_start);
  }
}

void RCchannel3 () {
  if (digitalRead(RC3PIN)) {
    channel3_start = micros();
  } else {
    channel3 = (uint16_t) (micros() - channel3_start);
  }
}
void RCchannel4 () {
  if (digitalRead(RC4PIN)) {
    channel4_start = micros();
  } else {
    channel4 = (uint16_t) (micros() - channel4_start);
  }
}

// Debug
void interruptDebug() {
  Serial.print("1: ");
  Serial.print(channel1);
  Serial.print(" 2: ");
  Serial.print(channel2);
  Serial.print(" 3: ");
  Serial.print(channel3);
  Serial.print(" 4: ");
  Serial.println(channel4);
}
