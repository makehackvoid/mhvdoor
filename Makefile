#Check the boards.txt file for the correct arduino version
ARDUINO_VERSION=mhvboard
TARGET=$(shell basename `pwd`)
AVRDUDE_PORT=usb

# you shouldn't need to change anything below here

AVRDUDE=avrdude

CC=avr-gcc
OBJ2HEX=avr-objcopy

MCU := $(shell grep $(ARDUINO_VERSION).build.mcu boards.txt | cut -d= -f2)
F_CPU := $(shell grep $(ARDUINO_VERSION).build.f_cpu boards.txt | cut -d= -f2)

AVRDUDE_PROGRAMER := $(shell grep $(ARDUINO_VERSION).upload.protocol boards.txt | cut -d= -f2)

# dirty hack to accomidate the arduino boards.txt file being wrong.
# can you believe they do this in the code rather than fixing their config file?
ifeq ($(AVRDUDE_PROGRAMER),stk500)
	AVRDUDE_PROGRAMER=stk500v1
endif

AVRDUDE_SPEED := $(shell grep $(ARDUINO_VERSION).upload.speed boards.txt | cut -d= -f2)

AVRDUDE_MAXSIZE := $(shell grep $(ARDUINO_VERSION).upload.max_size boards.txt | cut -d= -f2)

AVRDUDE_FLAGS = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMER) -b $(AVRDUDE_SPEED)
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex

CFLAGS := -g -Os -Wall -mcall-prologues -mmcu=$(MCU) -DF_CPU=$(F_CPU)

# deafult is to compile to the hex file
ALL : $(TARGET).hex
	
%.obj : %.o
	$(CC) $(CFLAGS) $< -o $@ -lm
	
%.hex : %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

clean :
	rm -f *.hex *.obj *.o

program: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) 

