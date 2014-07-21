// Send a new LED pattern to a LED node
void sendLED(uint8_t device_id, uint8_t r, uint8_t g, uint8_t b, uint8_t d) {  
  // Spawns a receiveEvent on the slave
  Wire.beginTransmission(device_id);
  Wire.write(r);
  Wire.write(g);
  Wire.write(b);
  Wire.write(d);
  Wire.endTransmission();
}
