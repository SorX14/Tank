// Set the compass up for continous measurement mode
void setupCompass() {
  Wire.beginTransmission(COMPASS);
  Wire.write(0x02);
  Wire.write(0x00);
  Wire.endTransmission();
}

// Get the compass direction, and also attach the last read time
void getCompass() {
  // Tell the device where to begin reading data
  Wire.beginTransmission(COMPASS);
  Wire.write(0x03); // Select register 3, X MSB register
  Wire.endTransmission();

  // Read data from each axis, 2 registers per axis
  Wire.requestFrom(COMPASS, 6);
  if (6 <= Wire.available()) {
    x_direction = Wire.read() << 8; // X MSB
    x_direction |= Wire.read(); // X LSB

    z_direction = Wire.read() << 8; // Z MSB
    z_direction |= Wire.read(); // Z LSB

    y_direction = Wire.read() << 8; // Y MSB
    y_direction |= Wire.read(); // Y LSB
  }

  compass_time = millis();
}

void setupAccelerometer() {
  Wire.beginTransmission(ACCELEROMETER);
  Wire.write(0x2D);
  Wire.write(8);
  Wire.endTransmission();
}

void getAccelerometer() {
  // Get the X axis
  Wire.beginTransmission(ACCELEROMETER);
  Wire.write(0x32); // X0
  Wire.write(0x33); // X1
  Wire.endTransmission();
  Wire.requestFrom(ACCELEROMETER, 2);
  if (Wire.available() <= 2) {
    x_angle = Wire.read() + (Wire.read() << 8);
  }

  // Get the Y axis
  Wire.beginTransmission(ACCELEROMETER);
  Wire.write(0x34); // Y0
  Wire.write(0x35); // Y1
  Wire.endTransmission();
  Wire.requestFrom(ACCELEROMETER, 2);
  if (Wire.available() <= 2) {
    y_angle = Wire.read() + (Wire.read() << 8);
  }

  // Get the Z axis
  Wire.beginTransmission(ACCELEROMETER);
  Wire.write(0x36); // Z0
  Wire.write(0x37); // Z1
  Wire.endTransmission();
  Wire.requestFrom(ACCELEROMETER, 2);
  if (Wire.available() <= 2) {
    z_angle = Wire.read() + (Wire.read() << 8);
  }

  // Output in g
  x_angle /= 256.0;
  y_angle /= 256.0;
  z_angle /= 256.0;
  accelerometer_time = millis();
}

void setupGyroscope() {
  // Setup normal power mode, all axis enabled
  Wire.beginTransmission(GYROSCOPE);
  Wire.write(0x20);
  Wire.write(0x0F);
  Wire.endTransmission();
}

void getGyroscope() {
  // Start reading
  Wire.beginTransmission(GYROSCOPE);
  Wire.write(0x28 | (1 << 7));
  Wire.endTransmission();

  // Request data from the gyroscope
  Wire.requestFrom(GYROSCOPE, 6);

  // Wait until we have 6 bytes in the buffer
  while (Wire.available() < 6);

  // Get each of the bytes
  uint8_t xla = Wire.read();
  uint8_t xha = Wire.read();
  uint8_t yla = Wire.read();
  uint8_t yha = Wire.read();
  uint8_t zla = Wire.read();
  uint8_t zha = Wire.read();

  // Output in degrees / secs
  x_rate = xha << 8 | xla;
  y_rate = yha << 8 | yla;
  z_rate = zha << 8 | zla;

  gyroscope_time = millis();
}
