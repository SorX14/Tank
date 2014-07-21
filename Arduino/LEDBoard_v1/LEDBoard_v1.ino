// Include libraries
#include <Wire.h>
#include <MilliTimer.h>

// Digital pins for RGB LED
#define RLED 5
#define GLED 3
#define BLED 6

// I2C and debug LED pins
#define I2CLED 2
#define DEBUGLED 4

// The I2CID pin
#define I2CID 7

// Fade start and end variables
#define SINSTART 4.712
#define SINEND 10.995

// Serial debug information
#define SER_DEBUG 1

// Struct
typedef struct {
  byte colors[3];
  byte steps;
} Ramp;

// Variables
int I2C_id;  // The I2C device ID
long now[3]; // Shamelessly stolen from JeeNodes LED
long delta[3];
word duration;
volatile uint8_t hascomms = 0; // Whether this unit has communication with the host

MilliTimer timer;
MilliTimer debug_timer;

void setup() {
  Serial.begin(115200);
  delay(500);
  
  // Setup all digital pins
  pinMode(I2CLED, OUTPUT);
  pinMode(DEBUGLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  pinMode(BLED, OUTPUT);
  pinMode(I2CID, INPUT_PULLUP); // Use a pinup to make sure its 3.3v, until its bridged
  
  // Testing
  //digitalWrite(DEBUGLED, HIGH);
   
  // Fix timer 1 so it also runs in fast PWM mode, to match timer 0
  bitSet(TCCR1B, WGM12);
  
  // Set the LED's up with R0 G0 B0 actioned immediately
  byte a[4] = { 0, 0, 0, 0 };
  useRamp((const Ramp*) a);
  
  // Determine I2C ID
  if (digitalRead(I2CID)) {
    I2C_id = 10;
  } else {
    I2C_id = 11;
  }
  
  // Start the I2C interface
  Wire.begin(I2C_id);
  Wire.onRequest(requestEvent); // Runs a function when a request comes in
  Wire.onReceive(receiveEvent); // Runs a function when a receive comes in
  
  Serial.println("LED BOARD");
  Serial.print("Starting as ID: ");
  Serial.println(I2C_id);
  Serial.println("Waiting for I2C master...");
  
  // While we don't have comms, flash the LED
  while (hascomms == 0) {
    if (debug_timer.poll(200)) {
      digitalWrite(I2CLED, !digitalRead(I2CLED));
    }
  }
  
  digitalWrite(I2CLED, HIGH);  
  Serial.println("Found master!");
}

void loop() {
  if (timer.poll(10)) {
    if (duration > 0) {
      --duration;
      for (byte i = 0; i < 3; ++i) {
        now[i] += delta[i];
      }
      setLEDs();
    }
  }
}

// As only masters can request, thats who asked for this data
void requestEvent() {
  Serial.println("requestEvent");
}

void receiveEvent(int in_length) {  
  // We are in communication with the controller board
  if (in_length == 0) {
    Serial.print("Heartbeat ");
    Serial.println(millis());
    hascomms = 1;
  } else {
    int i = 0;
    byte led_pattern[4];
    while (Wire.available()) {
      byte c = Wire.read();
      led_pattern[i] = c;
      i++;
    }
    
    useRamp((const Ramp*) led_pattern);
  }
}
