// Used code from https://github.com/mifay/Max32_CodeBase/blob/master/HMC5883L_LSM330/doc/HMC5883L/LoveElectronics

void setupAccelerometer() {
  if (!hasComms[3]) {
    return;
  }

  // Set range to 2
  // Get the current data from this register
  uint8_t data = readDevice(ACC_ADDRESS, 0x31, 1)[0];

  // We AND with 0xF4 to clear the bits we are going to set
  // Clearing ----X-XX
  data &= 0xF4;
  data |= 0x08; // Set full resolution bit

  // Write back to the device
  writeDevice(ACC_ADDRESS, 0x31, data);

  // Enable measurements
  writeDevice(ACC_ADDRESS, 0x2D, 0x08);
}

void setupCompass() {  
  if (!hasComms[4]) {
    return;
  }  
  // Sets the first (0x00) register to be 8 samples averaged, 15 hz, and normal measurement configuration
  writeDevice(COMPASS_ADDRESS, 0x00, 0x70);
  
  // Setup the scale to 1.3
  writeDevice(COMPASS_ADDRESS, 0x01, 0x20);

  // Set to continuous measurement mode
  writeDevice(COMPASS_ADDRESS, 0x02, 0x00);
}

void getAccelerometer() {
  if (!hasComms[3]) {
    return;
  }

  // Read 6 bytes from address 0x32, which is all of the axis information
  uint8_t* buffer = readDevice(ACC_ADDRESS, 0x32, 6);
  acc_x = ((buffer[1] << 8) | buffer[0]);
  acc_y = ((buffer[3] << 8) | buffer[2]);
  acc_z = ((buffer[5] << 8) | buffer[4]);

  // Now we have axis information, but we want it scaled and then filtered too
  facc_x = (acc_x * g_scale) * acc_alpha + (facc_x * (1.0 - acc_alpha));
  facc_y = (acc_y * g_scale) * acc_alpha + (facc_y * (1.0 - acc_alpha));
  facc_z = (acc_z * g_scale) * acc_alpha + (facc_z * (1.0 - acc_alpha));

  // Set the last read time
  acc_millis = millis();
}

void getCompass() {
  if (!hasComms[4]) {
    return;
  }
  
  // We have to ask the magnometer to return a value
  Wire.beginTransmission(COMPASS_ADDRESS);
  Wire.send(0x03);
  Wire.endTransmission();
  
  int16_t mag_x, mag_y, mag_z;
  
  Wire.requestFrom(COMPASS_ADDRESS, 6);
  if (6 <= Wire.available()) {
     mag_x = Wire.read() << 8;
     mag_x |= Wire.read();
     mag_z = Wire.read() << 8;
     mag_z |= Wire.read();
     mag_y = Wire.read() << 8;
     mag_y |= Wire.read();     
  }
   
  // Now we have readings, but we want it scaled and then filtered too
  fmag_x = (mag_x * guass_scale) * mag_alpha + (fmag_x * (1.0 - mag_alpha));
  fmag_y = (mag_y * guass_scale) * mag_alpha + (fmag_y * (1.0 - mag_alpha));
  fmag_z = (mag_z * guass_scale) * mag_alpha + (fmag_z * (1.0 - mag_alpha));
 
  // Set the last read time
  compass_millis = millis();
}

void getHeading() {
  // If we don't have either compass or accelerometer then bail
  if (!hasComms[3] || !hasComms[4]) {
    heading = 0;
    heading_nc = 0;
    return;
  }  
  
  // There are 2 types of heading, one compensated, and the other not
  // Non-compensated heading is easy
  heading_nc = atan2(fmag_y, fmag_x);
  heading_nc = RadiansToDegrees(heading_nc);
  
  // But the compensated one is hard
  // Some of these are used more than once so calculate once
  float cosRoll = cos(roll_r);
  float sinRoll = sin(roll_r);
  float cosPitch = cos(pitch_r);
  float sinPitch = sin(pitch_r);

  // The actual tilt compensation algorithem
  float Xh = fmag_x * cosPitch + fmag_z * sinPitch;
  float Yh = fmag_x * sinRoll * sinPitch + fmag_y * cosRoll - fmag_z * sinRoll * cosPitch;

  // Get the real heading
  heading = atan2(Yh, Xh);
  heading = RadiansToDegrees(heading);
}

void getPitchRoll() {
  if (!hasComms[3]) {
    roll_r = 0;
    pitch_r = 0;
    roll = 0;
    pitch = 0;
  }
  
  // Calculate the radian version of pitch/roll
  roll_r = atan2(facc_y, facc_z);
  pitch_r = atan2(facc_x, sqrt(facc_y * facc_y + facc_z * facc_z));
  
  // Now compute the degree version
  roll = roll_r * (180 / PI);
  pitch = pitch_r * (180 / PI);
}

double RadiansToDegrees(float rads) {
  // Magnetic declination: -1o 52' WEST (GL22BD)
  // -1 degrees 52 minutes to radians (Google to get radians conversion)
  float declinationAngle = -0.0325794794;
  rads += declinationAngle;

  // Correct for when signs are reversed
  if (rads < 0) {
    rads += 2 * PI;
  } 
  else if (rads > 2 * PI) { // Check for wrap
    rads -= 2 * PI;
  }

  // Convert radians to degrees for readability
  double h = rads * 180.0 / PI;

  return h;
}

