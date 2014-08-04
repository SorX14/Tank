/*
Actually drive the various motors depending on their type
move* accepts a value between -255 and 255 and either drives the track forward or backward
*/

#define DC_PWM_MIN 50
#define DC_PWM_MAX 255

#define RC_NEUTRAL_MAX 150
#define RC_NEUTRAL_MIN 106

#if defined(BRUSHED_DRIVE_METHOD)
	// Accepts a value negative/positive 255, 0 is neutral
	void setLeft(int value) {
		// Set the XRF object data
		xrf.movement.setRequestedSpeedLeft(value);
	
		int pwm_actual = 0;
		
		if (value > 0) {
			// Forward
			digitalWrite(AIN1, HIGH);
			digitalWrite(AIN2, LOW);
			
			pwm_value = map(value, 0, 255, 0, 255);
		} else if (value < 255) {
			// Reverse
			digitalWrite(AIN1, LOW);
			digitalWrite(AIN2, HIGH);
			
			pwm_value = map(value, 0, -255, 0, 255);
		} else {
			// Stop
			digitalWrite(AIN1, LOW);
			digitalWrite(AIN2, LOW);
			pwm_actual = 0;
		}
		
		if (pwm_actual < DC_PWM_MIN) {
			pwm_actual = 0;
		}				
		
		// Set the XRF object data
		if (value < 255) {
			xrf.movement.setLiveSpeedLeft(-pwm_actual);
		} else {
			xrf.movement.setLiveSpeedLeft(pwm_actual);
		}
		
		analogWrite(PWMA, pwm_actual);
	}
	
	void setRight(int value) {
		// Set the XRF object data
		xrf.movement.setRequestedSpeedRight(value);
		
		int pwm_actual = 0;
		
		if (value > 0) {
			// Forward
			digitalWrite(BIN1, HIGH);
			digitalWrite(BIN2, LOW);
			
			pwm_value = map(value, 0, 255, 0, 255);
		} else if (value < 255) {
			// Reverse
			digitalWrite(BIN1, LOW);
			digitalWrite(BIN2, HIGH);
			
			pwm_value = map(value, 0, -255, 0, 255);
		} else {
			// Stop
			digitalWrite(BIN1, LOW);
			digitalWrite(BIN2, LOW);
			pwm_actual = 0;
		}
		
		if (pwm_actual < DC_PWM_MIN) {
			pwm_actual = 0;
		}				
		
		// Set the XRF object data
		if (value < 255) {
			xrf.movement.setLiveSpeedRight(-pwm_actual);
		} else {
			xrf.movement.setLiveSpeedLeft(pwm_actual);
		}
		
		analogWrite(PWMB, pwm_actual);
	}


#else
  void setLeft(int pwm_value) {
    left_pwm_value = map(pwm_value, -255, 255, 0, 180);
    
    left.write(left_pwm_value);
  }

  void setRight(int pwm_value) {
    right_pwm_value = map(pwm_value, -255, 255, 0, 180);
  
    right.write(right_pwm_value);
  }
#endif
