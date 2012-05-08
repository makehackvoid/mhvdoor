#include <avr/io.h>
#include <util/delay.h>

#include <avr/pgmspace.h>

#define swap(value) asm volatile("swap %0" : "=r" (value) : "0" (value));

enum Mode {
  no_mode = 0,
  read_mode = 6,
  write_mode = 5,
  command_mode = 4
};

enum Command {
  disable_system_oscillator = 0,
  enable_system_oscillator = 1,
  leds_off = 2,
  leds_on = 3,
  blink_off = 8,
  blink_on = 9,
  slave_mode = 16,
  master_mode = 21,
  clock_source_internal = 24,
  clock_source_external = 28,
  nmos_8commons = 32,
  nmos_16commons = 36,
  pmos_8commons = 40,
  pmos_16commons = 44,
  pwm_1 = 160,
  pwm_2,
  pwm_3,
  pwm_4,
  pwm_5,
  pwm_6,
  pwm_7,
  pwm_8,
  pwm_9,
  pwm_10,
  pwm_11,
  pwm_12,
  pwm_13,
  pwm_14,
  pwm_15,
  pwm_16
};

// wclk rclk data
// B0   B1   B2

#define CS1_HIGH   (PORTD |=  _BV(PORTD3))
#define CS1_LOW    (PORTD &= ~_BV(PORTD3))
#define CS2_HIGH   (PORTD |=  _BV(PORTD5))
#define CS2_LOW    (PORTD &= ~_BV(PORTD5))
#define CS3_HIGH   (PORTD |=  _BV(PORTD6))
#define CS3_LOW    (PORTD &= ~_BV(PORTD6))
#define CS4_HIGH   (PORTD |=  _BV(PORTD7))
#define CS4_LOW    (PORTD &= ~_BV(PORTD7))
#define WCLK_HIGH (PORTB |=  _BV(PORTB0))
#define WCLK_LOW  (PORTB &= ~_BV(PORTB0))
#define RCLK_HIGH (PORTB |=  _BV(PORTB2))
#define RCLK_LOW  (PORTB &= ~_BV(PORTB2))
#define DATA_HIGH (PORTB |=  _BV(PORTB1))
#define DATA_LOW  (PORTB &= ~_BV(PORTB1))
#define DATA_OUT  (DDRB  |=  _BV(DDB1)  )
#define DATA_IN   (DDRB  &= ~_BV(DDB1)  )
#define DATA_READ (PINB  & _BV(PINB1))

#define SENSOR   (~PINC &   _BV(PINC0))
#define LED_ON   (PORTB |=  _BV(PORTB5))
#define LED_OFF  (PORTB &= ~_BV(PORTB5))

void write_bits_msb( uint8_t startingbit, uint8_t value ) {
  DATA_OUT;
  while (startingbit) {
    if( value & startingbit ) { DATA_HIGH; } else { DATA_LOW; }
    WCLK_LOW;
    WCLK_HIGH;
    startingbit >>= 1;
  }
}

void write_bits_lsb( uint8_t finishingbit, uint8_t value ) {
  finishingbit <<=1;
  DATA_OUT;
  uint8_t i = _BV(0);
  while (finishingbit != i) {
    if( value & i ) { DATA_HIGH; } else { DATA_LOW; }
    WCLK_LOW;
    WCLK_HIGH;
    i <<= 1;
  }
}

uint8_t read_bits_lsb() {
  uint8_t buffer = 0;
  DATA_IN;
  int i;
  for( i = 1; i < _BV(4); i<<=1 ) {
    RCLK_LOW;
    _delay_us(1);
    if( DATA_READ ) {
      buffer |= i;
    }
    RCLK_HIGH;
  }
  return buffer;
}
void send_address( uint8_t addr ) {
    write_bits_msb( _BV(6), addr );
}

void send_data( uint8_t data ) { // only sends bottom nibble
  write_bits_lsb( _BV(3), data );
}

void send_command( enum Command command ) {
  write_bits_msb( _BV(7), (uint8_t)command );
  write_bits_msb( _BV(0), 0 );
}

void set_mode(enum Mode mode) {
  write_bits_msb( _BV(2), (uint8_t)mode );
}

void write_buffer( uint8_t *buffer, uint8_t length, uint8_t offset ) {
  uint8_t *pointer = buffer;

  set_mode(write_mode);
  send_address( offset*2 );

  while( length-- ) {
    uint8_t temp = *pointer;
    send_data( temp );
    swap( temp );
    send_data( temp );
    ++pointer;
  }
}

void init() {
  set_mode(command_mode);
  send_command(disable_system_oscillator);
  send_command(pmos_8commons);
  send_command(master_mode);
  send_command(enable_system_oscillator);
  send_command(leds_on);
}
/*
void print_char( char c, uint8_t pos ) {
  uint8_t data[32], bitmap[32];

  int i;

  memcpy_P( data, &typeface[32*((c) -32)], 32 );
  for(i=0;i < 16; i++) {
   bitmap[i     ] = data[ i*2     ];
   bitmap[i + 16] = data[ i*2 + 1 ];
  }
 
  if( pos < 2 ) {
    CS1_LOW; 
    write_buffer( bitmap   , 16, pos & 1 ? 16 : 0 );
    CS1_HIGH;

    CS2_LOW;
    write_buffer( bitmap+16, 16, pos & 1 ? 16 : 0 );
    CS2_HIGH;
  } else { 
    CS3_LOW; 
    write_buffer( bitmap   , 16, pos & 1 ? 16 : 0 );
    CS3_HIGH;

    CS4_LOW;
    write_buffer( bitmap+16, 16, pos & 1 ? 16 : 0 );
    CS4_HIGH;
  }
}
*/
void blank() {
  set_mode(write_mode);
  send_address(0);
  int i;
  for(i = 0; i < 0x40; ++i)
    send_data(0x0);
}

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

  uint16_t OCCUPY = 0;
  while(1) {
    if( SENSOR ) {
      LED_ON;
      if( OCCUPY < 8*128 ) {
        OCCUPY++;
      }
    } else {
      LED_OFF;
      if(OCCUPY > 0) {
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
      if( (row+1) * 8 <= OCCUPY ) {
        send_data(0xFF);
        send_data(0xFF);
      } else if( row * 8 > OCCUPY ) {
        send_data(0x0);
        send_data(0x0);
      } else {
        uint8_t temp = 128;
        uint8_t lights_left = OCCUPY % 8;
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

