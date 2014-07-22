bool sendHeartbeatRequest(int device_id) {
  byte error;

  Wire.beginTransmission(device_id);
  error = Wire.endTransmission();

  if (error == 0) {
    return true;
  }
  return false;
}

void outputCommsList() {
  for (int i = 0; i < sizeof(hasComms); i++) {
    Serial.print("Device #");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(hasComms[i]);
  }
}

void getAvailableDevices() {
  hasComms[0] = sendHeartbeatRequest(VOLTAGE_DEVICE);
  hasComms[1] = sendHeartbeatRequest(LEFT_LED_DEVICE);
  hasComms[2] = sendHeartbeatRequest(RIGHT_LED_DEVICE);
  hasComms[3] = sendHeartbeatRequest(ACC_ADDRESS);
  hasComms[4] = sendHeartbeatRequest(COMPASS_ADDRESS);

  outputCommsList();
}

void getVoltage() {
	// I'm sure there are WAAAYYY more efficient ways of transferring data...
  Wire.requestFrom(VOLTAGE_DEVICE, 28);  // Request 28 bytes from the voltage board

  long previous_millis = 0;

  char vin_char[6];
  char v12_char[6];
  char v5_char[6];
  char v33_char[6];
  char percent_char[5];

  int i = 0;
  int p = 0;
  while (Wire.available()) {
    char c = Wire.read(); // receive a byte as character

    // Reset p on each comma
    if (c == ',') {
      p = 0;
      i++;
      continue;
    }

    if (i < 5) {
      vin_char[p] = c;
      p++;
      vin_char[p] = '\0';
    } else if (i > 5 && i < 11) {
      v12_char[p] = c;
      p++;
      v12_char[p] = '\0';
    } else if (i > 11 && i < 17) {
      v5_char[p] = c;
      p++;
      v5_char[p] = '\0';
    } else if (i > 17 && i < 23) {
      v33_char[p] = c;
      p++;
      v33_char[p] = '\0';
    } else if (i > 23) {
      percent_char[p] = c;
      p++;
      percent_char[p] = '\0';
    }
    i++;
  }

  vin = atol(vin_char);
  v12 = atol(v12_char);
  v5 = atol(v5_char);
  v33 = atol(v33_char);
  percent = atoi(percent_char);
}
