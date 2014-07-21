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

// ESC values
int esc_left = 0;
int esc_right = 0;

// Which devices we have comms with
bool hasComms[3];

// Voltages
unsigned long vin = 0;
unsigned long v12 = 0;
unsigned long v5 = 0;
unsigned long v33 = 0;

// Compass
int x_direction, y_direction, z_direction = 0;
unsigned long compass_time = 0;

// Accelerometer
double x_angle, y_angle, z_angle;
unsigned long accelerometer_time = 0;

// Gyroscope
double x_rate, y_rate, z_rate;
unsigned long gyroscope_time = 0;

#endif
