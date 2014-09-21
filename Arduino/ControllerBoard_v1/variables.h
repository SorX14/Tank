#ifndef variables_h
#define variables_h

#include <Arduino.h>

// Framerate
unsigned long framerate;

// Variables to hold all RC channels
volatile unsigned long channel1;
volatile unsigned long channel2;
volatile unsigned long channel3;
volatile unsigned long channel4;

// Variables to hold start points
// These don't need to be volatile as its only modified in the interrupt
unsigned long channel1_start;
unsigned long channel2_start;
unsigned long channel3_start;
unsigned long channel4_start;

// Variables to hold the constrained RC values
const float rc_alpha = 0.5; // Low-pass filter
int c1, c2, c3, c4;

// XRF comms
bool has_comms = false;

// Motor variables
int left_pwm_value;
int right_pwm_value;
bool parking_brake; // If true, the unit cannot move

// Control mode
enum ControlTypes {
	rc,
	xrf_radio
};
ControlTypes control_mode = xrf_radio;

// Which devices we have comms with
bool hasComms[6];

// Voltages in mV, won't be higher than 65530mV (hopefully)
unsigned int vin, v12, v5, v33, current, percent;

// IMU
double heading;              // Roll/pitch
int8_t roll, pitch;

// GPS
double longitude, latitude, speed_mph;
unsigned long fix_age;
unsigned int satellites;
bool has_fix;

#endif
