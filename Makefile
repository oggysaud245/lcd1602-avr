PORT=/dev/ttyACM0
MCU=atmega328p
F_CPU=16000000UL

CC=avr-g++
CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -I./lib
LDFLAGS=-Wl,-gc-sections,-relax

TARGET=main
SRC=main.cpp ./lib/lcd1602.cpp
OBJ=$(SRC:.cpp=.o)
ELF=$(TARGET).elf
HEX=$(TARGET).hex

all: $(HEX)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(ELF): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

$(HEX): $(ELF)
	avr-objcopy -O ihex -R .eeprom $(ELF) $(HEX)

flash: $(HEX)
	avrdude -p $(MCU) -c arduino -P $(PORT) -U flash:w:$(HEX):i

clean:
	rm -f $(OBJ) $(ELF)  # Delete object and ELF files, but leave the .hex file
