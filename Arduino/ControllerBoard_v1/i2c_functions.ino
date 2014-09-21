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
  hasComms[3] = sendHeartbeatRequest(CMPS_ADDRESS);

  outputCommsList();
}

void getVoltage() {
  // Only ask for the voltage if we can communicate with it
  if (hasComms[0] == 1) {
    Wire.requestFrom(VOLTAGE_DEVICE, 11);
    char buffer[11];
    int y = 0;
    while (Wire.available()) {
      byte c = Wire.read(); // receive a byte as character
      buffer[y] = c;
      y++;
    }
    
	// Get all of the ints from the bytes
     vin = (buffer[0] << 8) | buffer[1];
     v12 = (buffer[2] << 8) | buffer[3];
     v5 = (buffer[4] << 8) | buffer[5];
     v33 = (buffer[6] << 8) | buffer[7];
     current = (buffer[8] << 8) | buffer[9];
     percent = (int) buffer[10];
	 
	// Once we have the voltage, commit it to the XRF object
	xrf.voltage.setV33(v33);
	xrf.voltage.setV5(v5);
	xrf.voltage.setV12(v12);
	xrf.voltage.setVIN(vin);
	xrf.voltage.setCurrent(current);
	xrf.voltage.setBatPercent(percent);
  }
}

// Read a device after sending a data packet first
int16_t* readDevice (int I2CAddress, int address, int length) {
  Wire.beginTransmission(I2CAddress);
  Wire.write(address);
  Wire.endTransmission();

  return readDevice(I2CAddress, length);
}

// Read a device
int16_t* readDevice (int I2CAddress, int length) {
  Wire.requestFrom(I2CAddress, length);

  int16_t buffer[length];
  if (Wire.available() == length) {
    for (uint8_t i = 0; i < length; i++) {
      buffer[i] = Wire.read();
    }
  }

  return buffer;
}

// Write to a device
void writeDevice (int I2CAddress, int address, int data) {
  Wire.beginTransmission(I2CAddress);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}
