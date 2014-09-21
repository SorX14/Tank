// We now have access to a nice and quick tilt compensated compass with pitch and roll, so we can retrieve all the readings at once
void getSpatial() {
	// Don't try to get data if the module isn't plugged in
	if (!hasComms[3]) {
		return;
	}
	
	byte highByte, lowByte, fine;
        int bearing;
	
	Wire.beginTransmission(0x60);
	Wire.write(2);
	Wire.endTransmission();
	
	Wire.requestFrom(0x60, 4);
	while (Wire.available() < 4); // Just wait for the 4 bytes
	
	highByte = Wire.read();
	lowByte = Wire.read();
	pitch = Wire.read();
	roll = Wire.read();
	
	bearing = ((highByte << 8) + lowByte) / 10;
	fine = ((highByte << 8) + lowByte) % 10;
	heading = bearing + (fine / 100);
	
	xrf.imu.setPitch(pitch);
	xrf.imu.setRoll(roll);
	xrf.imu.setHeading(heading);
}
