# Makefile configuration
CPU              = msp430g2553
CFLAGS           = -mmcu=${CPU} -Os -I../h
LDFLAGS          = -L../lib -L/opt/ti/msp430_gcc/include/

# Compiler and Assembler
CC               = msp430-elf-gcc
AS               = msp430-elf-gcc -mmcu=${CPU} -c

# Target name
TARGET           = composite_state_machine.elf
OBJECTS          = composite_state_machine.o 

# Build all
all: $(TARGET)

# Linking
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lTimer -lLcd

# Load onto the device
load: $(TARGET)
	msp430loader.sh $^

# Clean build files
clean:
	rm -f *.o *.elf

