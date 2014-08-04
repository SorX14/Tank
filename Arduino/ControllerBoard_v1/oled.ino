void updateOLED() {
  display.clearDisplay();
  drawHeader();
  drawVoltage();
  drawCompass();
  draw3D();
  drawRC();
  display.display();
}

#define TL_BAT_X 117
#define TL_BAT_Y 1
#define TR_BAT_X 125
#define BR_BAT_Y 7

void drawHeader() {  
  // Draw the header line  
  for (int y = 0; y <= 8; y++) {
    display.drawLine(0, y, display.width(), y, WHITE);
  }

  // Inverted text
  display.setTextColor(BLACK, WHITE);
  
  // Add the clock in
  drawClock();
  
  // Add the motor drive type
  drawMotorType();
  
  // Add the signal
  drawSignalIcon();
  
  // Add the GPS fix icon
  drawGPSIcon();

  // Add the data icon
  drawDataIcon();

  // Draw the battery outline

  display.drawLine(TL_BAT_X, TL_BAT_Y, TR_BAT_X, TL_BAT_Y, BLACK); // TOP
  display.drawLine(TL_BAT_X, TL_BAT_Y, TL_BAT_X, BR_BAT_Y, BLACK); // LEFT
  display.drawLine(TL_BAT_X, BR_BAT_Y, TR_BAT_X, BR_BAT_Y, BLACK); // BOTTOM
  display.drawLine(TR_BAT_X, TL_BAT_Y + 1, TR_BAT_X + 1, TL_BAT_Y + 1, BLACK); 
  display.drawLine(TR_BAT_X, BR_BAT_Y - 1, TR_BAT_X + 1, BR_BAT_Y - 1, BLACK);
  display.drawLine(TR_BAT_X + 1, TL_BAT_Y + 1, TR_BAT_X + 1, BR_BAT_Y - 1, BLACK); // CAP  

  // Draw the actual battery shape
  display.setCursor(TL_BAT_X - 20, TL_BAT_Y);
  display.setTextSize(1);

  // Make sure the percent is properly displayed  
  if (percent < 10) {
    display.print(" ");
  } 
  display.print(percent);
  display.print("%");

  // Now show the bar graph  
  if (percent <= 20) {
    display.drawLine(TL_BAT_X + 2, TL_BAT_Y + 2, TL_BAT_X + 2, BR_BAT_Y - 2, BLACK); // 20%
  } 
  else if (percent <= 40) {
    display.drawLine(TL_BAT_X + 2, TL_BAT_Y + 2, TL_BAT_X + 2, BR_BAT_Y - 2, BLACK); // 20%
    display.drawLine(TL_BAT_X + 3, TL_BAT_Y + 2, TL_BAT_X + 3, BR_BAT_Y - 2, BLACK); // 40%
  } 
  else if (percent <= 60) {
    display.drawLine(TL_BAT_X + 2, TL_BAT_Y + 2, TL_BAT_X + 2, BR_BAT_Y - 2, BLACK); // 20%
    display.drawLine(TL_BAT_X + 3, TL_BAT_Y + 2, TL_BAT_X + 3, BR_BAT_Y - 2, BLACK); // 40%
    display.drawLine(TL_BAT_X + 4, TL_BAT_Y + 2, TL_BAT_X + 4, BR_BAT_Y - 2, BLACK); // 60%
  } 
  else if (percent <= 80) {
    display.drawLine(TL_BAT_X + 2, TL_BAT_Y + 2, TL_BAT_X + 2, BR_BAT_Y - 2, BLACK); // 20%
    display.drawLine(TL_BAT_X + 3, TL_BAT_Y + 2, TL_BAT_X + 3, BR_BAT_Y - 2, BLACK); // 40%
    display.drawLine(TL_BAT_X + 4, TL_BAT_Y + 2, TL_BAT_X + 4, BR_BAT_Y - 2, BLACK); // 60%
    display.drawLine(TL_BAT_X + 5, TL_BAT_Y + 2, TL_BAT_X + 5, BR_BAT_Y - 2, BLACK); // 80%
  } 
  else if (percent <= 90) {
    display.drawLine(TL_BAT_X + 2, TL_BAT_Y + 2, TL_BAT_X + 2, BR_BAT_Y - 2, BLACK); // 20%
    display.drawLine(TL_BAT_X + 3, TL_BAT_Y + 2, TL_BAT_X + 3, BR_BAT_Y - 2, BLACK); // 40%
    display.drawLine(TL_BAT_X + 4, TL_BAT_Y + 2, TL_BAT_X + 4, BR_BAT_Y - 2, BLACK); // 60%
    display.drawLine(TL_BAT_X + 5, TL_BAT_Y + 2, TL_BAT_X + 5, BR_BAT_Y - 2, BLACK); // 80%
    display.drawLine(TL_BAT_X + 6, TL_BAT_Y + 2, TL_BAT_X + 6, BR_BAT_Y - 2, BLACK); // 90%
  } 
  else {
    display.drawLine(TL_BAT_X + 2, TL_BAT_Y + 2, TL_BAT_X + 2, BR_BAT_Y - 2, BLACK); // 20%
    display.drawLine(TL_BAT_X + 3, TL_BAT_Y + 2, TL_BAT_X + 3, BR_BAT_Y - 2, BLACK); // 40%
    display.drawLine(TL_BAT_X + 4, TL_BAT_Y + 2, TL_BAT_X + 4, BR_BAT_Y - 2, BLACK); // 60%
    display.drawLine(TL_BAT_X + 5, TL_BAT_Y + 2, TL_BAT_X + 5, BR_BAT_Y - 2, BLACK); // 80%
    display.drawLine(TL_BAT_X + 6, TL_BAT_Y + 2, TL_BAT_X + 6, BR_BAT_Y - 2, BLACK); // 90%
    display.drawPixel(TL_BAT_X + 7, TL_BAT_Y + 3, BLACK); // 100%
  }
}

void drawMotorType() {
  display.setCursor(60, 1);
  display.setTextSize(1);
  #if defined(BRUSHED_DRIVE_METHOD)
    display.print("D");
  #else
    display.print("E");
  #endif
}

void drawGPSIcon() {
  if (has_fix) {
    // If we have a fix, show the solid icon
    display.drawBitmap(86, 1, gps_lock_icon, 7, 7, BLACK);
  } 
  else {
    // We don't have a fix, so flash it every 2 frames
    if (frameAction(2)) {
      display.drawBitmap(86, 1, gps_lock_icon, 7, 7, BLACK);
    }
  }
}

void drawSignalIcon() {
  int signal_pos = 70;
  display.drawBitmap(signal_pos, 1, signal_icon, 7, 7, BLACK);
  signal_pos += 4;
  display.drawLine(signal_pos, 7, signal_pos, 6, BLACK);
  signal_pos += 2;
  display.drawLine(signal_pos, 7, signal_pos, 5, BLACK);
  signal_pos += 2;
  display.drawLine(signal_pos, 7, signal_pos, 4, BLACK);
  signal_pos += 2;
  display.drawLine(signal_pos, 7, signal_pos, 3, BLACK);
}

void drawDataIcon() {

}

void drawVoltage() {
  display.setTextSize(1);
  display.setTextColor(WHITE);

  int line_y = 12;
  int line_x = 2;
  int line_space = 10;

  // Battery v
  display.setCursor(line_x, line_y);
  display.print("Bat:  ");
  display.print(vin / 1000.0, 1);
  display.print("v");

  line_y += line_space;

  // 12v
  display.setCursor(line_x, line_y);
  display.print("12v:  ");
  display.print(v12 / 1000.0, 1);
  display.print("v");

  line_y += line_space;

  // 5v
  display.setCursor(line_x, line_y);
  display.print("5v:   ");
  display.print(v5 / 1000.0, 1);
  display.print("v");

  line_y += line_space;

  // 3.3v
  display.setCursor(line_x, line_y);
  display.print("3.3v: ");
  display.print(v33 / 1000.0, 1);
  display.print("v");

  line_y += line_space;

  display.drawLine(0, line_y, display.width(), line_y, WHITE);
}

void drawClock() {
  display.setCursor(1, 1);
  display.setTextSize(1);

  // Output clock
  printDigits(hour());
  display.print(":");
  printDigits(minute());
  display.print(":");
  printDigits(second());
}

void drawCompass() {
  int compass_x = 90;
  int compass_y = 30;
  int radius = 15;

  // Draw the compass circle
  display.drawCircle(compass_x, compass_y, radius, WHITE);

  // Correct the starting point (360/0 should be at the top)
  int heading_int = heading + 270;
  if (heading_int > 360) {
    heading_int -= 360;
  }
  
  // Reverse the reading
  heading_int = map(heading_int, 0, 360, 360, 0);

  // Determine the compass line (angle, then the x/y endpoint)
  float angle = heading_int * PI / 180.0;
  float end_x = compass_x + cos(angle) * (radius - 3);
  float end_y = compass_y + sin(angle) * (radius - 3);

  // Draw the compass line
  display.drawLine(compass_x, compass_y, end_x, end_y, WHITE);
  
  // Add the 2 headings
  display.setCursor(110, 37);
  display.setTextSize(1);
  display.print((int) heading);
}

void draw3D() {
  display.setTextSize(1);

  display.setCursor(110, 17);
  display.print((int) roll);
  
  display.setCursor(110, 27);
  display.print((int) pitch);
}

void drawRC() {
  display.setTextSize(1);
  
  display.setCursor(0, 55);
  display.print(c1);
  display.setCursor(30, 55);
  display.print(c2);
  display.setCursor(60, 55);
  display.print(c3);
}
