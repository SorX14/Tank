void calibrateESC() {
  // Remove the reset interrupt as we'll need this for calibration
  detachInterrupt(RESET);
  
  // Set the outputs to neutral
  writeLeft(90);
  writeRight(90);  
  
  // Clear the screen
  display.clearDisplay();
  display.display();

  // Set a larger font size, and write starting, then waiting 2s
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(45, 0);
  display.println("ESC");
  display.setCursor(35, 20);
  display.println("SETUP");
  
  display.setCursor(7, 50);
  display.setTextSize(1);
  display.println("Press RESET to start");

  display.display();
  
  waitForReset();
  
  display.clearDisplay();  
  display.setTextSize(2);
  display.setCursor(37, 0);
  display.print("LEFT");
  
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.print("Hold SET, turn on ESC");
  display.setCursor(0, 30);
  display.print("Wait for flashing red");
  display.setCursor(7, 40);
  display.print("LED and release SET");
  display.setCursor(30, 55);
  display.print("Press RESET");
  display.display();
  
  waitForReset();
   
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(37, 0);
  display.print("LEFT");
  
  display.setTextSize(1);
  display.setCursor(8, 20);
  display.print("Press SET, wait for");
  display.setCursor(11, 30);
  display.print("1x green LED flash");
  
  display.setCursor(30, 55);
  display.print("Press RESET");
  display.display();
  
  waitForReset();
  
  // Set the output to full forward
  writeLeft(180);
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(37, 0);
  display.print("LEFT");
  
  display.setTextSize(1);
  display.setCursor(8, 20);
  display.print("Press SET, wait for");
  display.setCursor(11, 30);
  display.print("2x green LED flash");
  
  display.setCursor(30, 55);
  display.print("Press RESET");
  display.display();
  
  waitForReset();
  
  // Set the output to full reverse
  writeLeft(0);
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(37, 0);
  display.print("LEFT");
  
  display.setTextSize(1);
  display.setCursor(8, 20);
  display.print("Press SET, wait for");
  display.setCursor(11, 30);
  display.print("3x green LED flash");
  
  display.setCursor(30, 55);
  display.print("Press RESET");
  display.display();
  
  waitForReset();
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(32, 0);
  display.print("RIGHT");
  
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.print("Hold SET, turn on ESC");
  display.setCursor(0, 30);
  display.print("Wait for flashing red");
  display.setCursor(7, 40);
  display.print("LED and release SET");
  display.setCursor(30, 55);
  display.print("Press RESET");
  display.display();
  
  
  waitForReset();
    
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(32, 0);
  display.print("RIGHT");
  
  display.setTextSize(1);
  display.setCursor(8, 20);
  display.print("Press SET, wait for");
  display.setCursor(11, 30);
  display.print("1x green LED flash");
  
  display.setCursor(30, 55);
  display.print("Press RESET");
  display.display();
  
  waitForReset();
  
  // Set the output to full forward
  writeRight(180);
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(32, 0);
  display.print("RIGHT");
  
  display.setTextSize(1);
  display.setCursor(8, 20);
  display.print("Press SET, wait for");
  display.setCursor(11, 30);
  display.print("2x green LED flash");
  
  display.setCursor(30, 55);
  display.print("Press RESET");
  display.display();
  
  waitForReset();
  
  // Set the output to full reverse
  writeRight(0);
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(32, 0);
  display.print("RIGHT");
  
  display.setTextSize(1);
  display.setCursor(8, 20);
  display.print("Press SET, wait for");
  display.setCursor(11, 30);
  display.print("3x green LED flash");
  
  display.setCursor(30, 55);
  display.print("Press RESET");
  display.display();
  
  waitForReset();
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(45, 0);
  display.println("ESC");
  display.setCursor(35, 20);
  display.println("SETUP");
  display.setCursor(13, 40);
  display.println("COMPLETE!");
  display.display();
    
  delay(5000);  

  // Reattach the reset interrupt
  attachInterrupt(RESET, resetTeensy, FALLING);
}

void waitForReset() {
  delay(500);
  while (digitalRead(RESET) == HIGH) {}
  Serial1.println("Pressed");
  delay(250); // Poor mans debounce
}
