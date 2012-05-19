#include <math.h>
#include <avr/io.h>
#include <util/delay.h>

#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "ht1632.h"
#include "mhvdoor.h"
#include "colour.h"


volatile double OCCUPY = 0;
volatile float j = 0;


// Timmer routine gets called 100 times a second.
ISR(TIMER1_COMPA_vect) {
  if( SENSOR ) { LED_ON; }
  else { LED_OFF; }

  OCCUPY += SENSOR ? 0.01 : -0.01;
  
  if( OCCUPY < 0 ) OCCUPY = 0;
  if( OCCUPY > 84600 ) OCCUPY = 84600;

  j+= (18 + ((360-18)*(OCCUPY/84600)))*0.01;
  if( j > 360 ) j -= 360;
}

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


  // Set up the timer to tick every second
  cli();
  TCCR1A = 0;
  TCCR1B = 0;

  // set compare match register to desired timer count:
  OCR1A = 156;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS10) | (1 << CS12); // 1024 perscaler
  TIMSK1 |= (1 << OCIE1A);

  sei();

  // Setup the data for the LPD8806 based string of LEDs.
  uint8_t buffer[32*3];
  uint16_t i;
  for(i=0;i<32*3;i++) { buffer[i] = 0; }
  LPD8806_write(buffer,32*3);
  

  while(1) {
    
    for(i = 0; i < 32; ++i ) {
      HSVtoRGB(
        (int)((360/32)*i+j) % 360, 255, 255,
        buffer+1+i*3, buffer+0+i*3, buffer+2+i*3
      );
    }

    LPD8806_write(buffer,32*3);

    uint16_t leds = lrint(163.292 * log(22*OCCUPY/3600+1));

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
  }
  return 0;
}

