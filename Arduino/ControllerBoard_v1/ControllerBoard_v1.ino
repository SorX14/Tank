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
#include "TankXRF.h"

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
MilliTimer xrf_timer_slow;
MilliTimer xrf_timer_fast;

#if defined(BRUSHED_DRIVE_METHOD)
#else
// Setup servos
Servo left;
Servo right;
#endif 

// GPS
TinyGPSPlus gps;

// TankXRF
TankXRF xrf;

// Various I2C device IDs
#define VOLTAGE_DEVICE 5
#define LEFT_LED_DEVICE 10
#define RIGHT_LED_DEVICE 11

// CMPS10 Address
#define CMPS_ADDRESS 0x60

// RC constraint
#define RC_MIN 1070
#define RC_MAX 1900

// ESC constriant
#define ESC_MIN 1100
#define ESC_MAX 2000

// Failsafe cutoff
#define FAILSAFE_MS 200

void setup() {
  // Start the serial interface for debug, and GPS
  Serial.begin(115200);
  Serial1.begin(115200); // XRF
  Serial2.begin(9600); // Debug
  Serial3.begin(9600); // GPS

  Serial.println("STARTING...");
  delay(1000);
  
  // Setup time library
  setSyncProvider(getTeensy3Time);

  // Setup the XRF radio library
  xrf.init(Serial1);
  xrf.setDebug(Serial);

  // Start the I2C interface
  Wire.begin();

  // Setup the reset routine
  pinMode(RESET, INPUT);
  //attachInterrupt(RESET, resetTeensy, FALLING);

  // Setup RC controller inputs
  pinMode(RC1PIN, INPUT_PULLUP);
  pinMode(RC2PIN, INPUT_PULLUP);
  pinMode(RC3PIN, INPUT_PULLUP);
  pinMode(RC4PIN, INPUT_PULLUP);

  // We'll accept a maximum of 4 RC channels
  attachInterrupt(RC1PIN, RCchannel1, CHANGE);
  attachInterrupt(RC2PIN, RCchannel2, CHANGE);
  attachInterrupt(RC3PIN, RCchannel3, CHANGE);
  attachInterrupt(RC4PIN, RCchannel4, CHANGE);

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
    
    // Calibrate ESC
    //calibrateESC();
    
    // Make sure we're in neutral
    writeLeft(90);
    writeRight(90);
  #endif
}


void loop() {
  ///////////////////// GET AND SET MOTOR DATA
  // If we're using the RC
  if (control_mode == rc) { 
    // Conform the incoming RC signals
    conformRC();

    // c3 is a toggle switch
    if (c3 > 50) {
      setParkingBrake(true);
    } 
    else {
      setParkingBrake(false);
    }
    
    // Set the left and right motors
    setLeft(c1);
    setRight(c2);
  } else {
    // XRF control
    setLeft(xrf.control.left_y);
    setRight(xrf.control.right_y);
    
    if (xrf.control.cross) {
      sendLED(RIGHT_LED_DEVICE, 255, 0, 0, 1, 0);
      sendLED(LEFT_LED_DEVICE, 255, 0, 0, 1, 0);
    } else if (xrf.control.square) {
      sendLED(RIGHT_LED_DEVICE, 0, 50, 0, 1, 0);
      sendLED(LEFT_LED_DEVICE, 0, 50, 0, 1, 0);
    } else if (xrf.control.triangle) {
      sendLED(RIGHT_LED_DEVICE, 0, 0, 20, 1, 0);
      sendLED(LEFT_LED_DEVICE, 0, 0, 20, 1, 0);
    } else if (xrf.control.circle) {
      sendLED(RIGHT_LED_DEVICE, 0, 0, 0, 1, 0);
      sendLED(LEFT_LED_DEVICE, 0, 0, 0, 1, 0);
    } else {
    }
    
    // Failsafe, will turn off after 1 second of no comms
    if (xrf.comms.getLastMsg() > FAILSAFE_MS && has_comms == true) {
      has_comms = false;
      setParkingBrake(true);
      sendLED(RIGHT_LED_DEVICE, 0, 0, 0, 0, 1);
      sendLED(LEFT_LED_DEVICE, 0, 0, 0, 0, 1);      
    } else if (xrf.comms.getLastMsg() < FAILSAFE_MS && has_comms == false) {
      has_comms = true;
      setParkingBrake(false);
      sendLED(RIGHT_LED_DEVICE, 0, 0, 0, 0, 3);
      sendLED(LEFT_LED_DEVICE, 0, 0, 0, 0, 3); 
    }
    
//    Serial.println(xrf.comms.getLastMsg());
  }

  ////////////////////// GET SERIAL DATA

  // Get XRF data
  while (Serial1.available() > 0) {
    char x = Serial1.read();
    xrf.encode(x); 
  }

  // Get GPS data
  while (Serial3.available() > 0) {
    gps.encode(Serial3.read());
  }

  ////////////////////// GET SENSOR DATA

  // Communicate via I2C, and get the voltage from the voltage module
  if (i2c_timer.poll(250)) {
    // Get the voltage
    getVoltage();
    getGPS();
  }

  // Communicate with the IMU, and calculate the vehicles current position
  if (imu_timer.poll(100)) {
    // Get the IMU data
    getSpatial();
    
    // Update the screen
    updateOLED();
    framerate = 0;
  }
 
  if (control_mode == rc) { 
    // Send data that can wait
    if (xrf_timer_slow.poll(1000)) {
      xrf.sendSlow();
    }

    // Send data that we need quickly
    if (xrf_timer_fast.poll(100)) {  
      xrf.sendFast();
    }
  }
  
  // Add to the framerate
  framerate++;
}


