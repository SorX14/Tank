// Used code from https://github.com/mifay/Max32_CodeBase/blob/master/HMC5883L_LSM330/doc/HMC5883L/LoveElectronics

/* 
*	Sets the ADX3L345 to:
*	No self-test, 3 wire SPI, active high interrupts, full resolution (4mg/LSB), right-justified mode with sign extension, 16g range (ignored)
*	No link, no auto sleep, start measuring, stay awake
*/
void setupAccelerometer() {
  if (!hasComms[3]) {
    return;
  }

  /* Write back to the device
   Register 0x31 - DATA_FORMAT
   Bit 1 - SELF_TEST - Set to 1 for a shift in the output readings
   Bit 2 - SPI - Set to 1 for 3-wire, 0 for 4
   Bit 3 - INT_INVERT - Interrupts are either: 0 active high, 1 active low
   Bit 4 - N/A
   Bit 5 - FULL_RES - 1 for full res, or 0 for defined maximums
   Bit 6 - Justify - 1 left-justified (MSB) mode, 0 right-justified mode with sign extension
   Bit 7/8 - Range - 0 0 = 2g, 0 1 = 4g, 1 0 = 8g, 1 1 = 16g
  */
  writeDevice(ACC_ADDRESS, 0x31, 0x0B); // 0000 1011 

  /* Enable measurements
   Register 0x2D - POWER_CTL
   Bit 1/2 - N/A
   Bit 3 - Link - Read datasheet
   Bit 4 - AUTO_SLEEP - Set to 1 for autosleep, 0 disables
   Bit 5 - Measure - Set to 1 to measure, part in standby otherwise
   Bit 6 - Sleep - 0 normal mode, 1 sleep mode
   Bit 7/8 - Wakeup - Frequency of readings in sleep mode
  */
  writeDevice(ACC_ADDRESS, 0x2D, 0x08); // 0000 1000
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

/**
	We're in full resolution mode, so the range is 4mg/LSB
	+-16g fill resolution 13-bit. 13-bit unsigned number between 0 and 2^13 (8192)
	Actually its 3.90625mg/LSB according to the datasheet, and 16000 (16g) / 4096 (mid range)
	16g = 8192, 0g = 4096 and -16g = 0
	
	Help from: https://mbed.org/forum/mbed/topic/1069/?page=1#comment-5173
**/
void getAccelerometer() {
  if (!hasComms[3]) {
    return;
  }

  // Read 6 bytes from address 0x32, which is all of the axis information 
  // (the datasheet recommends grabbing the entire register in one read to
  // prevent the data changing during)
  uint8_t* buffer = readDevice(ACC_ADDRESS, 0x32, 6);
  acc_x = ((buffer[1] << 8) | buffer[0]);
  acc_y = ((buffer[3] << 8) | buffer[2]);
  acc_z = ((buffer[5] << 8) | buffer[4]);
  
  // Convert the values into G readings
  acc_x = (acc_x - 4096) / 256;
  acc_y = (acc_y - 4096) / 256;
  acc_z = (acc_z - 4096) / 256;

  // Now we have axis information, but we want it filtered to stop it from moving around so much
  facc_x = acc_x * acc_alpha + (facc_x * (1.0 - acc_alpha));
  facc_y = acc_y * acc_alpha + (facc_y * (1.0 - acc_alpha));
  facc_z = acc_z * acc_alpha + (facc_z * (1.0 - acc_alpha));
  
  // Set the last read time
  acc_millis = millis();
}

void getCompass() {
  if (!hasComms[4]) {
    return;
  }
  
  // We have to ask for a reading first, then get the result in one read. Order is definitely X then Z then Y
  uint8_t* buffer = readDevice(COMPASS_ADDRESS, 0x03, 6);
  mag_x = ((buffer[0] << 8) | buffer[1];
  mag_z = (buffer[2] << 8) | buffer[3];
  mag_y = (buffer[4] << 8) | buffer[5];
  
  /*
  
  // We have to ask the magnometer to return a value
  Wire.beginTransmission(COMPASS_ADDRESS);
  Wire.send(0x03);
  Wire.endTransmission();

  Wire.requestFrom(COMPASS_ADDRESS, 6);
  if (6 <= Wire.available()) {
     mag_x = Wire.read() << 8;
     mag_x |= Wire.read();
     mag_z = Wire.read() << 8;
     mag_z |= Wire.read();
     mag_y = Wire.read() << 8;
     mag_y |= Wire.read();     
  }
  */
   
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
	
	// Now commit to the XRF object
	xrf.imu.setHeading(heading);

    return;
  }  
    
  // There are 2 types of heading, one compensated, and the other not
  // Non-compensated heading is easy
  heading_nc = atan2(fmag_y, fmag_x);
  heading_nc = RadiansToDegrees(heading_nc + declinationAngle);
  
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
  heading = RadiansToDegrees(heading + declinationAngle);
  
  // Now commit to the XRF object
  xrf.imu.setHeading(heading);
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
  roll = RadiansToDegrees(roll_r);
  pitch = RadiansToDegrees(pitch_r);
  
  // Now commit to the XRF object
  xrf.imu.setPitch(pitch);
  xrf.imu.setRoll(roll);
}

double RadiansToDegrees(float rads) {
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

