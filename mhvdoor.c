#include <math.h>
#include <avr/io.h>
#include <util/delay.h>

#include <avr/pgmspace.h>
#include "ht1632.h"
#include "mhvdoor.h"

int main(void) {
  DDRB |= _BV(DDB5); // debug LED

  // make sure C0 is set to input and it's pullup is on
  DDRC  &= ~_BV(DDC0);
  PORTC |=  _BV(PORTC0);

  DDRB |= _BV(DDB0);
  DDRB |= _BV(DDB1);
  DDRB |= _BV(DDB2);
  
  DDRD |= _BV(DDD3); //cs1
  DDRD |= _BV(DDD5); //cs2
  DDRD |= _BV(DDD6); //cs3
  DDRD |= _BV(DDD7); //cs4

  PORTD |= _BV(PORTD3) | _BV(PORTD5) | _BV(PORTD6) | _BV(PORTD7);

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

  _delay_ms(10);

  double OCCUPY = 0;
  while(1) {
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

