#ifndef variables_h
#define variables_h

#include <Arduino.h>

// Variables to hold all RC channels
volatile uint16_t channel1;
volatile uint16_t channel2;
volatile uint16_t channel3;
volatile uint16_t channel4;

// Variables to hold start points
// These don't need to be volatile as its only modified in the interrupt
uint16_t channel1_start;
uint16_t channel2_start;
uint16_t channel3_start;
uint16_t channel4_start;

// Which devices we have comms with
bool hasComms[6];

// Voltages
unsigned long vin = 0;
unsigned long v12 = 0;
unsigned long v5 = 0;
unsigned long v33 = 0;
int percent = 0;

// IMU
float guass_scale = 0.92;		// Guass scaling
float g_scale = 0.0039; 			// G scaling

int16_t com_x, com_y, com_z;     // Compass values
const float com_alpha = 0.5;     // Compass low-pass filter
double fcom_x, fcom_y, fcom_z;    // Filtered compass values

int16_t acc_x, acc_y, acc_z;      // Accelerometer values
const float acc_alpha = 0.5;     // Acc low-pass filter 
double facc_x, facc_y, facc_z;   // Filtered accelerometer values

float heading_nc, heading;       // Compass heading
double roll_r, pitch_r;           // Roll/pitch radians
double roll, pitch;              // Roll/pitch

// IMU timers as to when data was last acquired
unsigned long acc_millis, gyro_millis, compass_millis;

// GPS
double longitude, latitude, speed_mph;
unsigned long fix_age;

#endif
