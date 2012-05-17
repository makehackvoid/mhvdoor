#ifndef __colour_h__
#define __colour_h__

#include <avr/io.h>

// Taken from http://www.nunosantos.net/archives/114
void HSVtoRGB( int h, int s, int v, uint8_t *r, uint8_t *g, uint8_t *b );

#endif
