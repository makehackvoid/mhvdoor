#include "ht1632.h"

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
