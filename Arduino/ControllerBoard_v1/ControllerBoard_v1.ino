// Include libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MilliTimer.h>
#include <Servo.h>

#include "pins.h"
#include "variables.h"

// Serial debugging
//#define SER_DEBUG 1

// Setup OLED SPI interface
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Timer to send heartbeat requests to I2C devices
MilliTimer i2c_timer;
MilliTimer oled_update;

// Setup servos
Servo left;
Servo right;

// Various I2C device IDs
#define VOLTAGE_DEVICE 5
#define LEFT_LED_DEVICE 10
#define RIGHT_LED_DEVICE 11

#define HMC5883L 0x1E
#define ADXL345 0x53
#define L3G4200D 0x69
#define BMP085 0x77

#define PRESSURE_SENSOR BMP085
#define GYROSCOPE L3G4200D
#define COMPASS HMC5883L
#define ACCELEROMETER ADXL345

// RC constraint
#define RC_MIN 1070
#define RC_MAX 1900

void setup() {
  // Start the serial interface
  Serial.begin(115200);

  // Start the I2C interface
  Wire.begin();

  pinMode(RC1PIN, INPUT);
  pinMode(RC2PIN, INPUT);
  pinMode(RC3PIN, INPUT);
  pinMode(RC4PIN, INPUT);

  // We'll accept a maximum of 4 RC channels
  attachInterrupt(RC1PIN, RCchannel1, CHANGE);
  attachInterrupt(RC2PIN, RCchannel2, CHANGE);
  attachInterrupt(RC3PIN, RCchannel3, CHANGE);
  attachInterrupt(RC4PIN, RCchannel4, CHANGE);

  // Setup motors
  left.attach(5);
  right.attach(6);

  // Setup the OLED display with a blank display
  display.begin();
  display.clearDisplay();
  display.display();

  // Set a larger font size, and write starting, then waiting 2s
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(15, 0);
  display.println("STARTING!");
  display.display();
  delay(2000);

  // Setup a smaller font size
  display.setTextSize(1);

  // Get available I2C devices
  getAvailableDevices();

  // Setup the IMU
  setupCompass();
  setupAccelerometer();
  setupGyroscope();

  // Draw the OLED header
  drawHeader();
}


void loop() {
  // Convert the RC channel values to 0-255 integers
  int c1 = constrain(map(channel1, RC_MIN, RC_MAX, 0, 255), 0, 255);
  int c2 = constrain(map(channel2, RC_MIN, RC_MAX, 0, 255), 0, 255);
  int c3 = constrain(map(channel3, RC_MIN, RC_MAX, 0, 255), 0, 255);
  int c4 = constrain(map(channel4, RC_MIN, RC_MAX, 0, 255), 0, 255);

  // Every 250ms, get the voltage, compass, accelerometer, gyrometer and OLED
  if (oled_update.poll(250)) {
    // Get the voltage
    getVoltage();

    // Get the IMU
    getCompass();
    getAccelerometer();
    getGyroscope();

    // Update the OLED
    updateOLED();
  }
  
  

  // Output the speed to the ESC's
  left.write(esc_left);
  right.write(esc_right);
}
