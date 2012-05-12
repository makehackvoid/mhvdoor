#ifndef __ht1632_h__
#define __ht1632_h__

#include "mhvdoor.h"
#include <avr/io.h>
#include <util/delay.h>

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

void write_bits_msb( uint8_t startingbit, uint8_t value );
void write_bits_lsb( uint8_t finishingbit, uint8_t value );
uint8_t read_bits_lsb();
void send_address( uint8_t addr );
void send_data( uint8_t data ); // only sends bottom nibble
void send_command( enum Command command );
void set_mode(enum Mode mode);
void write_buffer( uint8_t *buffer, uint8_t length, uint8_t offset );
void init();
//void print_char( char c, uint8_t pos );
void blank();

#endif
