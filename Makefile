# MCU settings
MCU = atmega328p
F_CPU = 16000000UL
PROGRAMMER = arduino
PORT = COM3

# Compiler settings
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Wextra
LDFLAGS = -mmcu=$(MCU)

# Source files
SOURCES = main.c dht.c lcd.c servo.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = main

# Default target
all: $(TARGET).hex

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files
$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# Create hex file
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Program MCU
flash: $(TARGET).hex
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -U flash:w:$<:i

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET).elf $(TARGET).hex

.PHONY: all flash clean
