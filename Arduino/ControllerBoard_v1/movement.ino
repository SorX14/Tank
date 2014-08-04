/*
Actually drive the various motors depending on their type
move* accepts a value between -255 and 255 and either drives the track forward or backward
*/

#define DC_PWM_MIN 50
#define DC_PWM_MAX 255

#define RC_NEUTRAL_MAX 150
#define RC_NEUTRAL_MIN 106

#if defined(BRUSHED_DRIVE_METHOD)
  void setLeft(int pwm_value) {
     int pwm_actual = 0;
     
    // Above neutral zone, go forward
    if (pwm_value > RC_NEUTRAL_MAX) {
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
      pwm_actual = map(pwm_value, RC_NEUTRAL_MAX, 255, DC_PWM_MIN, DC_PWM_MAX);
    } else if (pwm_value < RC_NEUTRAL_MIN) {
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
      pwm_actual = map(pwm_value, 0, RC_NEUTRAL_MIN, DC_PWM_MAX, DC_PWM_MIN);
    } else {
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, LOW);
      pwm_actual = 0;
    }
    
    Serial.print("LEFT: ");
    Serial.print(pwm_actual);
    
    analogWrite(PWMA, pwm_actual);
  }
  
  void setRight(int pwm_value) {
    int pwm_actual = 0;
    
    // Above neutral zone, go forward
    if (pwm_value > RC_NEUTRAL_MAX) {
      digitalWrite(BIN2, HIGH);
      digitalWrite(BIN1, LOW);
      pwm_actual = map(pwm_value, RC_NEUTRAL_MAX, 255, DC_PWM_MIN, DC_PWM_MAX);
    } else if (pwm_value < RC_NEUTRAL_MIN) {
      digitalWrite(BIN2, LOW);
      digitalWrite(BIN1, HIGH);
      pwm_actual = map(pwm_value, 0, RC_NEUTRAL_MIN, DC_PWM_MAX, DC_PWM_MIN);
    } else {
      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, LOW);
      pwm_actual = 0;
    }
    
    Serial.print(" RIGHT: ");
    Serial.println(pwm_actual);
        
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
