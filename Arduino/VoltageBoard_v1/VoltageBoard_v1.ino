// Include libraries
#include <Wire.h>
#include <MilliTimer.h>

// Define the I2C LED and ID
#define I2CLED 2
#define I2CID 5

// Setup battery voltage indications
#define BAT25   10
#define BAT50   9
#define BAT75   6
#define BAT100  5

// Setup the analog inputs
#define AVIN   A0
#define A12V   A1
#define A5V    A2
#define A33V   A3

// Setup the calibration values
#define CAL_VIN 0.046667
#define CAL_V12 0.081667
#define CAL_V5 0.186
#define CAL_V33 0.296969

volatile uint8_t hascomms = 0; // Whether this unit has communication with the host

MilliTimer debug_timer;
MilliTimer voltage_flash;

// Setup the analog readers
const float alpha = 0.5;
unsigned long r_v12, r_v5, r_v33, r_vin;
volatile unsigned long v12, v5, v33, vin;
volatile int percent = 0;

void setup() {
  Serial.begin(115200);
  delay(500);
  
  // Setup analog read reference
  analogReference(INTERNAL);
  
  // Setup all digital pins
  pinMode(I2CLED, OUTPUT);
  pinMode(BAT25, OUTPUT);
  pinMode(BAT50, OUTPUT);
  pinMode(BAT75, OUTPUT);
  pinMode(BAT100, OUTPUT);
  
  // Setup up inputs
  pinMode(AVIN, INPUT);
  pinMode(A12V, INPUT);
  pinMode(A5V, INPUT);
  pinMode(A33V, INPUT);
  
  // Start the I2C interface
  Wire.begin(I2CID);
  Wire.onRequest(requestEvent); // Runs a function when a request comes in
  Wire.onReceive(receiveEvent); // Runs a function when a receive comes in
  
  Serial.println("VOLTAGE BOARD");
  Serial.print("Starting as ID: ");
  Serial.println(I2CID);
  Serial.println("Waiting for I2C master...");
  
  // While we don't have comms, flash the LED
  while (hascomms == 0) {
    if (debug_timer.poll(200)) {
      digitalWrite(I2CLED, !digitalRead(I2CLED));
    }
    
    // Read the current voltages
    readVoltages();
    
    // Output the voltage with the LEDs
    outputVoltage();
  }
  
  digitalWrite(I2CLED, HIGH);  
  Serial.println("Found master!");
}

void loop() {
    // Read the current voltages
    readVoltages();
    
    // Output the voltage with the LEDs
    outputVoltage();
}

void requestEvent() {
  // Output VIN, 12v, 5v, 3.3v
  if (hascomms) {
    char vstring[28];
    sprintf(vstring, "%05lu,%05lu,%05lu,%05lu,%03d", vin, v12, v5, v33, percent);
    Serial.println(vstring);
    Wire.write(vstring, 28);
  }
}

void receiveEvent(int in_length) {
  if (in_length == 0) {
    Serial.print("Heartbeat ");
    Serial.println(millis());
    hascomms = 1;
  } else if (hascomms) {
    Serial.print("receiveEvent ");
    Serial.print(in_length);
    Serial.println("b:");
    
    while (Wire.available()) {
      byte c = Wire.read();
      Serial.print((int) c);
    }
    Serial.println();
  }

}
