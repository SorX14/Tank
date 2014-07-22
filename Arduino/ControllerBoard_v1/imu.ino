// Used code from https://github.com/mifay/Max32_CodeBase/blob/master/HMC5883L_LSM330/doc/HMC5883L/LoveElectronics

void setupAccelerometer() {
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
  // Setup the scale
  uint8_t regValue = 0x01; // Sets the scale to 1.3 gauss
  regValue = regValue << 5;
  writeDevice(COMPASS_ADDRESS, 0x01, regValue);
    
  // Set to continuous measurement mode
  writeDevice(COMPASS_ADDRESS, 0x02, 0x00);
}


void getAccelerometer() {
	// Read 6 bytes from address 0x32, which is all of the axis information
	uint16_t* buffer = readDevice(ACC_ADDRESS, 0x32, 6);
	acc_x = ((buffer[1] << 8) | buffer[0]) * g_scale;
	acc_y = ((buffer[3] << 8) | buffer[2]) * g_scale;
	acc_z = ((buffer[5] << 8) | buffer[4]) * g_scale;
	
	// Now we have scaled axis information, but we want it filtered too
	facc_x = acc_x * acc_alpha + (facc_x * (1.0 - acc_alpha));
	facc_y = acc_y * acc_alpha + (facc_y * (1.0 - acc_alpha));
	facc_z = acc_z * acc_alpha + (facc_z * (1.0 - acc_alpha));
  
	// Set the last read time
	acc_millis = millis();
}

void getCompass() {
  // Request the data
  uint8_t* buffer = readDevice(COMPASS_ADDRESS, 0x03, 6);
  com_x = ((buffer[0] << 8) | buffer[1]) * guass_scale;
  com_z = ((buffer[2] << 8) | buffer[3]) * guass_scale;
  com_y = ((buffer[4] << 8) | buffer[5]) * guass_scale;
      
  // Now we have scaled readings, but we want it filtered too
  fcom_x = com_x * com_alpha + (fcom_x * (1.0 - com_alpha));
  fcom_y = com_y * com_alpha + (fcom_y * (1.0 - com_alpha));
  fcom_z = com_z * com_alpha + (fcom_z * (1.0 - com_alpha));
      
  // Set the last read time
  compass_millis = millis();
}

void getPosition() {
	// Calculate the heading
  heading_nc = atan2(fcom_y, fcom_x);
  
  // Get degrees out and compensation for magnetic deinclination
  heading_nc = RadiansToDegreesMG(heading_nc);

  // We need both the compass and the accelerometer readings
  //roll_r = asin(facc_y);
  //pitch_r = asin(facc_x);
  roll_r = atan2(facc_y, facc_z);
  pitch_r = atan2(facc_x, sqrt(facc_y * facc_y + facc_z * facc_z));
  
  // Convert radians to degrees
  roll = roll_r * (180 / PI);
  pitch = pitch_r * (180 / PI);
  
  // We cannot compensate for angles over 40 degrees as this is the simple version
  if (roll_r > 0.78 || roll_r < -0.78 || pitch_r > 0.78 || pitch_r < -0.78) {
    heading = 0;
  } else {
    // Some of these are used more than once so calculate once
    float cosRoll = cos(roll_r);
    float sinRoll = sin(roll_r);
    float cosPitch = cos(pitch_r);
    float sinPitch = sin(pitch_r);
    
    // The actual tilt compensation algorithem
    float Xh = fcom_x * cosPitch + fcom_z * sinPitch;
    float Yh = fcom_x * sinRoll * sinPitch + fcom_y * cosRoll - fcom_z * sinRoll * cosPitch;
    
    heading = atan2(Yh, Xh);
	
	// Get degrees out and compensation for magnetic deinclination
	heading = RadiansToDegreesMG(heading);
  }
}


uint8_t* readDevice (int I2CAddress, int address, int length) {
	Wire.beginTransmission(I2CAddress);
	Wire.send(address);
	Wire.endTransmission();
	
	Wire.requestFrom(I2CAddress, length);
	
	uint16_t buffer[length];
	if (Wire.available() == length) {
		for (uint8_t i = 0; i < length; i++) {
			buffer[i] = Wire.read();
		}
	}
	
	return buffer;
}

void writeDevice (int I2CAddress, int address, int data) {
	Wire.beginTransmission(I2CAddress);
	Wire.send(address);
	Wire.send(data);
	Wire.endTransmission();
}

double RadiansToDegreesMG(float rads) {
	// Magnetic declination: -1o 52' WEST (GL22BD)
    // -1 degrees 52 minutes to radians (Google to get radians conversion)
    float declinationAngle = -0.0325794794;
    rads += declinationAngle;

	// Correct for when signs are reversed
	if (rads < 0) {
		rads += 2 * PI;
	} else if (rads > 2 * PI) { // Check for wrap
		rads -= 2 * PI;
	}
	
	// Convert radians to degrees for readability
	double heading = rads * 180 / PI;
	
	return heading;
}
