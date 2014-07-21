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
bool hasComms[3];

// Voltages
unsigned long vin = 0;
unsigned long v12 = 0;
unsigned long v5 = 0;
unsigned long v33 = 0;
int percent = 0;

// IMU
double acc_x, acc_y, acc_z;
int16_t com_x, com_y, com_z;
float heading;

// IMU timers as to when data was last acquired
unsigned long acc_millis, gyro_millis, compass_millis;

#endif
