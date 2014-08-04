void conformRC() {
  // Convert the RC channel values to 0-255 integers
  c1 = constrain(map(channel1, RC_MIN, RC_MAX, 0, 255), 0, 255);
  c2 = constrain(map(channel2, RC_MIN, RC_MAX, 0, 255), 0, 255);
  c3 = constrain(map(channel3, RC_MIN, RC_MAX, 0, 255), 0, 255);
  c4 = constrain(map(channel4, RC_MIN, RC_MAX, 0, 255), 0, 255);
  
  
}
