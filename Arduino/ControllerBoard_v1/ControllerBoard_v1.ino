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

#include "pins.h"
#include "variables.h"

// Serial debugging
//#define SER_DEBUG 1

// Setup OLED SPI interface
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Timer to send heartbeat requests to I2C devices
MilliTimer i2c_timer;
MilliTimer oled_timer;
MilliTimer imu_timer;
MilliTimer debug_timer;

// Setup servos
Servo left;
Servo right;

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
  Serial3.begin(9600);
  
  // Start the I2C interface
  Wire.begin();

  // Setup time library  
  setSyncProvider(getTeensy3Time);
  
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
  display.setCursor(15, 20);
  display.println("STARTING!");
  display.display();
  delay(2000);
  
  // Setup a smaller font size
  display.setTextSize(1);
  
  // Get available I2C devices
  getAvailableDevices();  
  
  // Setup IMU
  setupAccelerometer();
  setupCompass();
}


void loop() {  
	// Convert the RC channel values to 0-255 integers
	int c1 = constrain(map(channel1, RC_MIN, RC_MAX, 0, 255), 0, 255);
	int c2 = constrain(map(channel2, RC_MIN, RC_MAX, 0, 255), 0, 255);
	int c3 = constrain(map(channel3, RC_MIN, RC_MAX, 0, 255), 0, 255);
	int c4 = constrain(map(channel4, RC_MIN, RC_MAX, 0, 255), 0, 255);
  
	// Get GPS data
	while (Serial3.available() > 0) {
		gps.encode(Serial3.read());
	}
 
	// Communicate via I2C, and get the voltage from the voltage module
	if (i2c_timer.poll(250)) {
		// Get the voltage
		getVoltage();
	}
  
	// Communicate with the IMU, and calculate the vehicles current position
	if (imu_timer.poll(100)) {
		// Get the IMU data
		getAccelerometer();
		getCompass();
		getGPS();
	
		getPosition(); // Calculates heading, pitch and roll
	}
 
	// Update the OLED, with 100ms, there will be a 10Hz refresh rate
	if (oled_timer.poll(100)) {
		// Update the screen
		updateOLED();
	}
	
	// Any debug operations
	if (debug_timer.poll(500)) {
		// Output the GPS situation
		Serial.print("GPS Lng: ");
		Serial.print(longitude, 6);
		Serial.print(" Lat: ");
		Serial.print(latitude, 6);
		Serial.print(" Speed: ");
		Serial.print(speed_mph);
		Serial.print("MPH");
		Serial.println();
	}
}