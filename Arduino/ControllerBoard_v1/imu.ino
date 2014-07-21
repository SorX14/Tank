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
  
  // Set the last read time
  acc_millis = millis();
}

void setupCompass() {
  Wire.beginTransmission(COMPASS_ADDRESS);
  Wire.send(0x02); // Select mode register
  Wire.send(0x00); // Continuous measurement mode
  Wire.endTransmission();
}

void getCompass() {
  Wire.beginTransmission(COMPASS_ADDRESS);
  Wire.send(0x03); //select register 3, X MSB register
  Wire.endTransmission();
 
  Wire.requestFrom(COMPASS_ADDRESS, 6);
  if( 6 <= Wire.available()){
    com_x = (Wire.read() << 8) | Wire.read();
    com_z = (Wire.read() << 8) | Wire.read();
    com_y = (Wire.read() << 8) | Wire.read();
  }
  
  // Calculate the heading
  heading = atan2(com_y, com_x);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: 2o 37' W, which is 2.617 Degrees, or (which we need) 0.0456752665 radians, I will use 0.0457
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  //float declinationAngle = 0.0457;
  
  // Magnetic declination: -1° 52' WEST (GL22BD)
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
  
  // Set the last read time
  compass_millis = millis();
}
