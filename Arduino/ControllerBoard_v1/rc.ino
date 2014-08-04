void conformRC() {	
  // Make sure the values are constrained between 0 and 255 and mapped to min and max values for the motors
  int c1_pre = constrain(map(channel1, RC_MIN, RC_MAX, -255, 255), -255, 255);
  int c2_pre = constrain(map(channel2, RC_MIN, RC_MAX, -255, 255), -255, 255);
  int c3_pre = constrain(map(channel3, RC_MIN, RC_MAX, -255, 255), -255, 255);
  int c4_pre = constrain(map(channel4, RC_MIN, RC_MAX, -255, 255), -255, 255);

  // Apply a basic low-pass filter to remove some of the jitter
  c1 = c1_pre * rc_alpha + (c1_pre * (1.0 - rc_alpha));
  c2 = c2_pre * rc_alpha + (c2_pre * (1.0 - rc_alpha));
  c3 = c3_pre * rc_alpha + (c3_pre * (1.0 - rc_alpha));
  c4 = c4_pre * rc_alpha + (c4_pre * (1.0 - rc_alpha));

  // It would appear that the Turnigy 9X receiver will continue to output a signal
  // regardless of whether there is a transmitter attached, this signal needs to be
  // calibrated and then checked
  
  
  // Check to see if any of the channels haven't been updated for 20ms or longer
  if ((uint16_t) (micros() - channel1_start) > 20000) {
    c1 = 0;
  }

  if ((uint16_t) (micros() - channel2_start) > 20000) {
    c2 = 0;
  }

  if ((uint16_t) (micros() - channel3_start) > 20000) {
    c3 = 0;
  }

  if ((uint16_t) (micros() - channel4_start) > 20000) {
    c4 = 0;
  }
}



