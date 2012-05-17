#include <math.h>
#include <avr/io.h>
#include <util/delay.h>

#include <avr/pgmspace.h>
#include "ht1632.h"
#include "mhvdoor.h"
#include "colour.h"


int main(void) {
  DDRB |= _BV(DDB5); // debug LED

  // PIR Sensor input
  // make sure C0 is set to input and it's pullup is on
  DDRC  &= ~_BV(DDC0);
  PORTC |=  _BV(PORTC0);

  // HT1632 data bus
  DDRB |= _BV(DDB0);
  DDRB |= _BV(DDB1);
  DDRB |= _BV(DDB2);
  
  // HT1632 Chip Selects
  DDRD |= _BV(DDD3); //cs1
  DDRD |= _BV(DDD5); //cs2
  DDRD |= _BV(DDD6); //cs3
  DDRD |= _BV(DDD7); //cs4

  // Pull all the Chip Selects on the HT1632s high
  PORTD |= _BV(PORTD3) | _BV(PORTD5) | _BV(PORTD6) | _BV(PORTD7);
  
  // LPD8806 data bus
  DDRC |= _BV(DDC4);
  DDRC |= _BV(DDC5);

  // Setup each of the HT1632 displays
  CS1_LOW;
  init();
  CS1_HIGH;
 
  CS2_LOW;
  init();
  CS2_HIGH;
 
  CS3_LOW;
  init();
  CS3_HIGH;
 
  CS4_LOW;
  init();
  CS4_HIGH;
 
  CS1_LOW;
  blank();
  CS1_HIGH;

  CS2_LOW;
  blank();
  CS2_HIGH;

  CS3_LOW;
  blank();
  CS3_HIGH;

  CS4_LOW;
  blank();
  CS4_HIGH;

  // Setup the data for the LPD8806 based string of LEDs.
  uint8_t buffer[32*3];
  uint16_t i;
  for(i=0;i<32*3;i++) { buffer[i] = 0; }
  LPD8806_write(buffer,32*3);
  
  uint16_t j=0;

  double OCCUPY = 0;
  while(1) {
    
    for(i = 0; i < 32; ++i ) {
      HSVtoRGB(
        ((360/32)*i+j) % 360, 255, 255,
        buffer+1+i*3, buffer+0+i*3, buffer+2+i*3
      );
    }
    j+=8;

    LPD8806_write(buffer,32*3);

    if( SENSOR ) {
      LED_ON;
      if( OCCUPY < 84600 ) {
        OCCUPY++;
      }
    } else {
      LED_OFF;
      if(OCCUPY > 1) {
        OCCUPY--;
      }
    }

    uint16_t row;
    for( row = 0; row < 128; row ++ ) {
      switch( row ){
        case 0:
          CS4_HIGH;
          CS1_LOW;
          set_mode(write_mode);
          send_address(0);
          break;
        case 32:
          CS1_HIGH;
          CS2_LOW;
          set_mode(write_mode);
          send_address(0);
          break;
        case 64:
          CS2_HIGH;
          CS3_LOW;
          set_mode(write_mode);
          send_address(0);
          break;
        case 96:
          CS3_HIGH;
          CS4_LOW;
          set_mode(write_mode);
          send_address(0);
          break;
      }
      uint16_t leds = lrint(163.292 * log(22*OCCUPY/3600+1));
      if( (row+1) * 8 <= leds ) {
        send_data(0xFF);
        send_data(0xFF);
      } else if( row * 8 > leds ) {
        send_data(0x0);
        send_data(0x0);
      } else {
        uint8_t temp = _BV(7);
        uint8_t lights_left = leds % 8;
        while( lights_left > 0 ) {
          --lights_left;
          temp >>= 1;
          temp |= 128;
        }
        send_data( temp );
        swap(temp);
        send_data( temp );
      }
    }
    CS4_HIGH;
    _delay_ms(1000);

  }
  return 0;
}

