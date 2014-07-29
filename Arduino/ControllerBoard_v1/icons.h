#ifndef icons_h
#define icons_h

#include <Arduino.h>

/*
Use LCD Assistant found @ http://en.radzio.dxp.pl/bitmap_converter

And make sure that the width is a multiple of 8 (everything else will be truncated)
The height doesn't matter

Size endianness:   LITTLE
Pixels/byte:       8

*/

// 7x7 image of GPS lock (horizontal, little)
static const unsigned char PROGMEM gps_lock_icon [] = {
0x38, 0x54, 0x82, 0xD6, 0x82, 0x54, 0x38, 0x00, 
};

// 7x7 signal antenna
static const unsigned char PROGMEM signal_icon [] = {
0xF8, 0xA8, 0x70, 0x20, 0x20, 0x20, 0x20, 
};


#endif
