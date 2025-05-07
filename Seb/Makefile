all: libSeb.a sebdemo.elf

CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -Os -I../h 
LDFLAGS         = -L/opt/ti/msp430_gcc/include -L../lib 
CC              = msp430-elf-gcc
AS              = msp430-elf-as
AR              = msp430-elf-ar

# Static library
libSeb.a: sebutils.o sebmath.o
	$(AR) crs $@ $^

# Object dependencies
sebutils.o: sebutils.c sebutils.h
sebmath.o: sebmath.c sebmath.h

# Install headers and library
install: libSeb.a
	mkdir -p ../h ../lib
	mv $^ ../lib
	cp *.h ../h

# Clean all build artifacts
clean:
	rm -f libSeb.a *.o *.elf

# Demo executable
sebdemo.elf: sebdemo.o libSeb.a
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -lTimer

# Flash to device
load: sebdemo.elf
	msp430loader.sh $^
