#include "colour.h"

// Taken from http://www.nunosantos.net/archives/114
void HSVtoRGB( int h, int s, int v, uint8_t *r, uint8_t *g, uint8_t *b ) {
  int f;
  long p, q, t;

  if( s == 0 ) {
    *r = *g = *b = v;
    return;
  }

  f = ((h%60)*255)/60;
  h /= 60;

  p = (v * ( 256 - s ) ) / 256;
  q = (v * ( 256 - ( s * f ) / 256 ) ) / 256;
  t = (v * ( 256 - ( s * ( 256 - f ) ) / 256 ) ) / 256;

  p >>= 1;
  q >>= 1;
  t >>= 1;
  v >>= 1;

  switch( h ) {
    case 0:
      *r = v;
      *g = t;
      *b = p;
    break;
    case 1:
      *r = q;
      *g = v;
      *b = p;
    break;
    case 2:
      *r = p;
      *g = v;
      *b = t;
    break;
    case 3:
      *r = p;
      *g = q;
      *b = v;
    break;
    case 4:
      *r = t;
      *g = p;
      *b = v;
    break;
    default:
      *r = v;
      *g = p;
      *b = q;
    break;
  }
}

