#ifndef variables_h
#define variables_h

#include <Arduino.h>

// Framerate
unsigned long framerate;

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

// Variables to hold the constrained RC values
const float rc_alpha = 0.5; // Low-pass filter
int c1, c2, c3, c4;

// Motor variables
int left_pwm_value;
int right_pwm_value;
bool parking_brake; // If true, the unit cannot move

// Control mode
enum ControlTypes {
	rc,
	xrf_radio
};
ControlTypes control_mode = rc;

// Which devices we have comms with
bool hasComms[6];

// Voltages in mV, won't be higher than 65530mV (hopefully)
unsigned int vin, v12, v5, v33, percent;

// IMU
// Magnetic declination: -1o 52' WEST (GL22BD)
// -1 degrees 52 minutes to radians (Google to get radians conversion)
const float declinationAngle = -0.0325794794;

float guass_scale = 0.92;		// Guass scaling
float g_scale = 0.0039; 			// G scaling

int16_t mag_x, mag_y, mag_z;     // Compass values
const float mag_alpha = 0.5;     // Compass low-pass filter
double fmag_x, fmag_y, fmag_z;    // Filtered compass values

int16_t acc_x, acc_y, acc_z;      // Accelerometer values
const float acc_alpha = 0.4;     // Acc low-pass filter 
double facc_x, facc_y, facc_z;   // Filtered accelerometer values

float heading_nc, heading;       // Compass heading
double roll_r, pitch_r;           // Roll/pitch radians
double roll, pitch;              // Roll/pitch

// IMU timers as to when data was last acquired
unsigned long acc_millis, gyro_millis, compass_millis;

// GPS
double longitude, latitude, speed_mph;
unsigned long fix_age;
unsigned int satellites;
bool has_fix;

#endif
