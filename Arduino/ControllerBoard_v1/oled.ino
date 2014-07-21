void updateOLED() {
  
}

void drawHeader() {
  display.clearDisplay();
  display.display();
  
  // Draw the header line  
  display.drawLine(0, 8, display.width(), 8, WHITE);
  
  // Draw the battery outline
  #define TL_BAT_X 118
  #define TL_BAT_Y 1
  #define TR_BAT_X 126
  #define BR_BAT_Y 7
    
  display.drawLine(TL_BAT_X, TL_BAT_Y, TR_BAT_X, TL_BAT_Y, WHITE); // TOP
  display.drawLine(TL_BAT_X, TL_BAT_Y, TL_BAT_X, BR_BAT_Y, WHITE); // LEFT
  display.drawLine(TL_BAT_X, BR_BAT_Y, TR_BAT_X, BR_BAT_Y, WHITE); // BOTTOM
  display.drawLine(TR_BAT_X, TL_BAT_Y + 1, TR_BAT_X + 1, TL_BAT_Y + 1, WHITE); 
  display.drawLine(TR_BAT_X, BR_BAT_Y - 1, TR_BAT_X + 1, BR_BAT_Y - 1, WHITE);
  display.drawLine(TR_BAT_X + 1, TL_BAT_Y + 1, TR_BAT_X + 1, BR_BAT_Y - 1, WHITE); // CAP  
  display.display();
  
  
}

// display.fillRect(i, i, display.width()-i*2, display.height()-i*2, color%2);
//    display.display();
