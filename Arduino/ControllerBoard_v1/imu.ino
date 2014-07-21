void setupAccelerometer() {
  Wire.beginTransmission(ACC_ADDRESS);
  Wire.write(0x2D); // ?
  Wire.write(8); // Measure enable
  Wire.endTransmission();
}

void getAccelerometer() {
  /////--------------------------------------- GET X AXIS
  Wire.beginTransmission(ACC_ADDRESS);
  Wire.write(0x32);
  Wire.endTransmission();
  
  Wire.requestFrom(ACC_ADDRESS, 2);
  int16_t X_out;
  if (Wire.available() <= 2) {
    X_out = (int16_t) (Wire.read() | (Wire.read() << 8));
  }
  
  /////--------------------------------------- GET Y AXIS
  Wire.beginTransmission(ACC_ADDRESS);
  Wire.write(0x34);
  Wire.endTransmission();
  
  Wire.requestFrom(ACC_ADDRESS, 2);
  int16_t Y_out;
  if (Wire.available() <= 2) {
    Y_out = (int16_t) (Wire.read() | (Wire.read() << 8));
  }
  
  /////--------------------------------------- GET Z AXIS
  Wire.beginTransmission(ACC_ADDRESS);
  Wire.write(0x36);
  Wire.endTransmission();
  
  Wire.requestFrom(ACC_ADDRESS, 2);
  int16_t Z_out;
  if (Wire.available() <= 2) {
    Z_out = (int16_t) (Wire.read() | (Wire.read() << 8));
  }

  // Calculate in G
  acc_x = X_out / 256.0;  
  acc_y = Y_out / 256.0;
  acc_z = Z_out / 256.0;
  
  // Now filter the readings
  facc_x = acc_x * acc_alpha + (facc_x * (1.0 - acc_alpha));
  facc_y = acc_y * acc_alpha + (facc_y * (1.0 - acc_alpha));
  facc_z = acc_z * acc_alpha + (facc_z * (1.0 - acc_alpha));
  
  // Set the last read time
  acc_millis = millis();
}

void setupCompass() {
  Wire.beginTransmission(COMPASS_ADDRESS);
  Wire.write(0x02); // Select mode register
  Wire.write(0x00); // Continuous measurement mode
  Wire.endTransmission();
}

void getCompass() {
  Wire.beginTransmission(COMPASS_ADDRESS);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
 
  Wire.requestFrom(COMPASS_ADDRESS, 6);
  if( 6 <= Wire.available()){
    com_x = (Wire.read() << 8) | Wire.read();
    com_z = (Wire.read() << 8) | Wire.read();
    com_y = (Wire.read() << 8) | Wire.read();
  }
  
  // Now filter the readings
  fcom_x = com_x * com_alpha + (fcom_x * (1.0 - com_alpha));
  fcom_y = com_y * com_alpha + (fcom_y * (1.0 - com_alpha));
  fcom_z = com_z * com_alpha + (fcom_z * (1.0 - com_alpha));
  
  // Calculate the heading
  heading_nc = atan2(fcom_y, fcom_x);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: 2o 37' W, which is 2.617 Degrees, or (which we need) 0.0456752665 radians, I will use 0.0457
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  //float declinationAngle = 0.0457;
  
  // Magnetic declination: -1o 52' WEST (GL22BD)
  // -1 degrees 52 minutes to radians (Google to get radians conversion)
  float declinationAngle = -0.0325794794;
  heading_nc += declinationAngle;
  
  // Correct for when we have negatives
  if (heading_nc < 0) {
    heading_nc += 2*PI;
  }
  if (heading_nc > 2*PI) {
    heading_nc -= 2*PI;
  }
  
  // Convert radians to degress
  heading_nc *= 180/M_PI;
  
  // Set the last read time
  compass_millis = millis();
}

void getHeading() {
  // We need both the compass and the accelerometer readings
  roll_r = asin(facc_y);
  pitch_r = asin(facc_x);
  
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
    
    // Magnetic declination: -1o 52' WEST (GL22BD)
    // -1 degrees 52 minutes to radians (Google to get radians conversion)
    float declinationAngle = -0.0325794794;
    heading += declinationAngle;
  
    // Correct for when we have negatives
    if (heading < 0) {
      heading += 2*PI;
    }
    if (heading > 2*PI) {
      heading -= 2*PI;
    }
  
    // Convert radians to degress
    heading *= 180/M_PI;
  }
}
