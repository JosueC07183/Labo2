MCU=attiny4313
F_CPU=1000000
CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-std=c99 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
TARGET=laboratorio2
SRCS=laboratorio2.c

all: build doxygen run

build:
	${CC} ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex

flash:
	avrdude -p ${MCU} -c usbasp -U flash:w:${TARGET}.hex:i -F -P usb

doxygen:
	doxygen -g
	doxygen

run:
	open html/index.html

clean:
	rm -f *.c~ *.h~ *.o *.elf *.hex *.asm *.cod *.lst *.bin
	rm -rf html latex Doxyfile