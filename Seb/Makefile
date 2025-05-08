# Makefile configuration
CPU             = msp430g2553
CFLAGS = -I./timerLib
CFLAGS = -I./lcdLib
CFLAGS          = -mmcu=${CPU} -Os -I../h
LDFLAGS         = -L../lib -L/opt/ti/msp430_gcc/include/

# Switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

# Rule to copy files
copy_files:
    cp -r $(SRC_DIR)/* $(DEST_DIR)/

#additional rules for files
wakedemo.elf: ${COMMON_OBJECTS} stateMachine.o wdt_handler.o sound.o soundbuttons.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd 

# Source files
OBJS            = soundbuttons.o stateMachine.o sound.o wdt_handler.o wakedemo.c

# Target
TARGET          = soundbuttons.elf

all: ${TARGET}

# Link the final ELF binary
${TARGET}: ${OBJS}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd

# Load the program to the board
load: ${TARGET}
	msp430loader.sh $^

# Clean all build files
clean:
	rm -f *.o *.elf
