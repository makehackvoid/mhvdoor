#ifndef __mhvdoor_h__
#define __mhvdoor_h__

#include "ht1632.h"
#include "lpd8806.h"
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

#define STRIP_DATA_HIGH (PORTC |=  _BV(PORTC4))
#define STRIP_DATA_LOW  (PORTC &= ~_BV(PORTC4))
#define STRIP_CLK_HIGH  (PORTC |=  _BV(PORTC5))
#define STRIP_CLK_LOW   (PORTC &= ~_BV(PORTC5))

#endif
