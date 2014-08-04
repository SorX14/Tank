// Include libraries
#include <SPI.h>
//#include <i2c_t3.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MilliTimer.h>
#include <Servo.h>
#include <Time.h>
#include "TinyGPS++.h"

#define BRUSHED_DRIVE_METHOD 1

#include "pins.h"
#include "variables.h"
#include "icons.h"

// Serial debugging
//#define SER_DEBUG 1

// Setup OLED SPI interface
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Code to software reset Teensy
#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);

// Timer to send heartbeat requests to I2C devices
MilliTimer i2c_timer;
MilliTimer oled_timer;
MilliTimer imu_timer;
MilliTimer debug_timer;

#if defined(BRUSHED_DRIVE_METHOD)
#else
  // Setup servos
  Servo left;
  Servo right;
#endif 


// GPS
TinyGPSPlus gps;

// Various I2C device IDs
#define VOLTAGE_DEVICE 5
#define LEFT_LED_DEVICE 10
#define RIGHT_LED_DEVICE 11

// GY-80 I2C addresses
#define ACC_ADDRESS 0x53
#define COMPASS_ADDRESS 0x1E

// RC constraint
#define RC_MIN 1070
#define RC_MAX 1900

void setup() {
  // Start the serial interface for debug, and GPS
  Serial.begin(115200);
  Serial1.begin(9600); // XRF
  Serial2.begin(9600); // Debug
  Serial3.begin(9600); // GPS
  
  delay(1000);
  Serial.println("STARTING...");

  // Start the I2C interface
  Wire.begin();

  // Setup time library
  setSyncProvider(getTeensy3Time);

  // Setup the reset routine
  pinMode(RESET, INPUT);
  attachInterrupt(RESET, resetTeensy, FALLING);

  // Setup RC controller inputs
  pinMode(RC1PIN, INPUT);
  pinMode(RC2PIN, INPUT);
  pinMode(RC3PIN, INPUT);
  pinMode(RC4PIN, INPUT);

  // We'll accept a maximum of 4 RC channels
  attachInterrupt(RC1PIN, RCchannel1, CHANGE);
  attachInterrupt(RC2PIN, RCchannel2, CHANGE);
  attachInterrupt(RC3PIN, RCchannel3, CHANGE);
  attachInterrupt(RC4PIN, RCchannel4, CHANGE);

  #if defined(BRUSHED_DRIVE_METHOD)
    Serial.println("Setting up brushed motor control");
    pinMode(PWMA, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(PWMB, OUTPUT);
  #else
    // Setup motors
    Serial.println("Setting up ESC motor control");
    left.attach(5);
    right.attach(6);
  #endif

  // Setup the OLED display with a blank display
  display.begin();
  display.clearDisplay();
  display.display();
  
  // Reset all connected devices
  resetI2CDevices();

  // Set a larger font size, and write starting, then waiting 2s
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(15, 20);
  display.println("STARTING!");
  display.display();
  delay(1000);

  // Setup a smaller font size
  display.setTextSize(1);

  // Get available I2C devices
  getAvailableDevices();

  // Setup IMU
  setupAccelerometer();
  setupCompass();
}


void loop() {
  ///////////////////// GET AND SET MOTOR DATA
  conformRC();
  if (c3 > 100) {
    setLeft(c1);
    setRight(c2);
  } else {
    setLeft(128);
    setRight(128);
  } 
    
  ////////////////////// GET SERIAL DATA
 
  // Get XRF data
  while (Serial1.available() > 0) {
    char x = Serial1.read();
    //Serial.print(x);
  }
  
  // Get debug serial data
  while (Serial2.available() > 0) {
    char y = Serial2.read();
    Serial.print(y);
  }
  
  // Get GPS data
  while (Serial3.available() > 0) {
    gps.encode(Serial3.read());
  }
  
  ////////////////////// GET SENSOR DATA

  // Communicate via I2C, and get the voltage from the voltage module
  if (i2c_timer.poll(250)) {
    // Get the voltage
    getVoltageSlim();
  }

  // Communicate with the IMU, and calculate the vehicles current position
  if (imu_timer.poll(100)) {
    // Get the IMU data
    getAccelerometer();
    getCompass();
    getGPS();

    getPitchRoll(); // Calculate the pitch and roll which is needed for...
    getHeading(); // Calculating heading
        
    // Debug the heading
//    Serial.print("Heading:\t");
//    Serial.print(heading_nc);
//    Serial.print(",");
//    Serial.print(heading);
//    Serial.print("\t");
//    Serial.print(right_pwm_value);
//    Serial.println();
  }

  // Update the OLED, with 100ms, there will be a 10Hz refresh rate
  if (oled_timer.poll(100)) {
    // Update the screen
    updateOLED();
  }

  // Any debug operations
  if (debug_timer.poll(15)) {

  }
  

}
