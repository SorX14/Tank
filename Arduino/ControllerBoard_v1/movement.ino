/*
Actually drive the various motors depending on their type
move* accepts a value between -255 and 255 and either drives the track forward or backward
*/

#define DC_PWM_MIN 200
#define DC_PWM_MAX 255


#if defined(BRUSHED_DRIVE_METHOD)
  // Convert the PWM value to positive and limited to motor turn on level
  int pwmConverter(int pwm_value) {
    int abs_pwm_value = abs(pwm_value);
    
//    if (abs_pwm_value < DC_PWM_MIN) {
//      abs_pwm_value = 0;
//    } else {
      abs_pwm_value = map(abs_pwm_value, 0, 255, DC_PWM_MIN, DC_PWM_MAX);
    //}
    
    return abs_pwm_value;
  }

  void setLeft(int pwm_value) {
    // If the PWM value is positive, we'll be clockwise
    if (pwm_value > 0) {    
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
    } else {
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
    }
    
    int pwm_out = pwmConverter(pwm_value);
    
    Serial.println(pwm_out);
    
    analogWrite(PWMA, pwm_out);
  }
  
  void setRight(int pwm_value) {
    // If the PWM value is positive, we'll be clockwise
    if (pwm_value > 0) {
      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, LOW);
    } else {
      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, HIGH);
    }
    
    analogWrite(PWMB, pwmConverter(pwm_value));
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
