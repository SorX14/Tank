void readVoltages() {
  analogRead(AVIN);
  delay(5);
  vin = ((analogRead(AVIN) * (1100/1023.0))) / CAL_VIN;
  
  analogRead(A12V);
  delay(5);
  v12 = ((analogRead(A12V) * (1100/1023.0))) / CAL_V12;
  
  analogRead(A5V);
  delay(5);
  v5 = ((analogRead(A5V) * (1100/1023.0))) / CAL_V5;
  
  analogRead(A33V);
  delay(5);
  v33 = ((analogRead(A33V) * (1100/1023.0))) / CAL_V33;
}

void outputVoltage() { 
  if (vin > 20150) {
    digitalWrite(BAT25, HIGH);
    digitalWrite(BAT50, HIGH);
    digitalWrite(BAT75, HIGH);
    analogWrite(BAT100, map(vin, 20150, 21000, 0, 255));
    percent = map(vin, 20150, 21000, 75, 100);
  } else if (vin > 19300) {
    digitalWrite(BAT25, HIGH);
    digitalWrite(BAT50, HIGH);
    analogWrite(BAT75, map(vin, 19300, 20150, 0, 255));
    digitalWrite(BAT100, LOW);    
    percent = map(vin, 19300, 20150, 50, 75);
  } else if (vin > 18950) {
    digitalWrite(BAT25, HIGH);
    analogWrite(BAT50, map(vin, 18950, 19300, 0, 255));
    digitalWrite(BAT75, LOW);
    digitalWrite(BAT100, LOW);
    percent = map(vin, 18950, 19300, 25, 50);
  } else if (vin > 18000) {
    analogWrite(BAT25, map(vin, 18000, 18950, 0, 255));
    digitalWrite(BAT50, LOW);
    digitalWrite(BAT75, LOW);
    digitalWrite(BAT100, LOW);
    percent = map(vin, 18000, 18950, 0, 25);
  } else {
    if (voltage_flash.poll(100)) {
      digitalWrite(BAT25, !digitalRead(BAT25));
    }
    digitalWrite(BAT50, LOW);
    digitalWrite(BAT75, LOW);
    digitalWrite(BAT100, LOW);
    percent = 0;
  }

  /*  
  Battery voltage table
  ======================
[  4.20v   100%   21.00v
  
[  4.03v   76%    20.15v
  
[  3.86v   52%    19.30v
[  3.83v   42%    19.15v
  
[  3.79v   30%    18.95v
[  3.70v   11%    18.50v
[  3.6v    0%     18.00v
  */
}
