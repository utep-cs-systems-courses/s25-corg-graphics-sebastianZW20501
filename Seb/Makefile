CC = msp430-gcc
CPU = msp430g2553
CFLAGS = -mmcu=$(CPU) -I/opt/ti/msp430_gcc/include -L/opt/ti/msp430_gcc/include -Os
LIBS = -lTimer -lLcd

# Source files
SRC = soundbuttons.c sound.c stateMachine.c wdt_handler.s
OBJ = $(SRC:.c=.o)

# Executable
TARGET = soundbuttons.elf

# Default target
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

wdt_handler.o: wdt_handler.s
	msp430-as -mmcu=$(CPU) wdt_handler.s -o wdt_handler.o

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	msp430-run $(TARGET)

flash: $(TARGET)
	msp430-loader.sh $(TARGET)

load: $(TARGET)
	msp430-load $(TARGET)

.PHONY: all clean run flash load
