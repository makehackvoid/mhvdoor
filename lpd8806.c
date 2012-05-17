#include "lpd8806.h"

void LPD8806_write(uint8_t *buffer, uint16_t count) {
  uint16_t i;
  uint8_t *pointer = buffer;
  uint8_t bit;

  for (i=0; i<count; i++ ) {
    STRIP_DATA_HIGH;
    STRIP_CLK_HIGH;
    STRIP_CLK_LOW;
    for (bit=0x40; bit; bit >>= 1) {
      if(*pointer & bit) STRIP_DATA_HIGH;
      else               STRIP_DATA_LOW;
      STRIP_CLK_HIGH;
      STRIP_CLK_LOW;
    }
    pointer++;
  }
    
  STRIP_DATA_LOW;
  for(i = 8 * 4; i>0; i--) {
    STRIP_CLK_HIGH;
    STRIP_CLK_LOW;
  }

  _delay_ms(3);
}
