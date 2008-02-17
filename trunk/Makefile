# Name: Makefile
# Author: Tim Stegeman
# Creation Date: 2008-02-16

DEVICE = attiny2313
AVRDUDE = avrdude -p $(DEVICE)
# Choose your favorite programmer and interface above.
F_CPU = 12000000
COMPILE = avr-gcc -Wall -Os -Iusbtiny -I. -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) #-DDEBUG_LEVEL=2
# NEVER compile the final product with debugging! Any debug output will
# distort timing so that the specs can't be met.

OBJECTS = usbtiny/crc.o usbtiny/int.o usbtiny/usb.o main.o

# symbolic targets:
all:		main.hex

.c.o:
		$(COMPILE) -c $< -o $@

.S.o:
		$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
		$(COMPILE) -S $< -o $@

flash:		all
		$(AVRDUDE) -U flash:w:main.hex:i

fuses:
		$(AVRDUDE) -U hfuse:w:0xdb:m -U lfuse:w:0xef:m

clean:
		rm -f *.hex *.o *.bin usbtiny/*.o

main.bin:	$(OBJECTS)
		$(COMPILE) -o main.bin $(OBJECTS)

main.hex:	main.bin
		rm -f main.hex
		avr-objcopy -j .text -j .data -O ihex main.bin main.hex
		avr-size main.bin

disasm:		main.bin
		avr-objdump -d main.bin

cpp:
		$(COMPILE) -E main.c

