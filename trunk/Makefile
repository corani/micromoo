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
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
		$(COMPILE) -S $< -o $@

flash:		all
		$(AVRDUDE) -U flash:w:main.hex:i

# Fuse low byte:
# 0xef = 1 1 1 0   1 1 1 1
#        ^ ^ \+/   \--+--/
#        | |  |       +------- CKSEL 3..0 (clock selection -> crystal @ 12 MHz)
#        | |  +--------------- SUT 1..0 (BOD enabled, fast rising power)
#        | +------------------ CKOUT (clock output on CKOUT pin -> disabled)
#        +-------------------- CKDIV8 (divide clock by 8 -> don't divide)
#
# Fuse high byte:
# 0xdb = 1 1 0 1   1 0 1 1
#        ^ ^ ^ ^   \-+-/ ^
#        | | | |     |   +---- RSTDISBL (disable external reset -> enabled)
#        | | | |     +-------- BODLEVEL 2..0 (brownout trigger level -> 2.7V)
#        | | | +-------------- WDTON (watchdog timer always on -> disable)
#        | | +---------------- SPIEN (enable serial programming -> enabled)
#        | +------------------ EESAVE (preserve EEPROM on Chip Erase -> not preserved)
#        +-------------------- DWEN (debug wire enable)
fuses:		# only needed for attiny2313
		$(AVRDUDE) -U hfuse:w:0xdb:m -U lfuse:w:0xef:m

clean:
		rm -f main.hex main.lst main.obj main.cof main.list main.map main.eep.hex main.bin main.elf *.o usbtiny/*.o main.s usbtiny/*.s

# file targets:
main.bin:	$(OBJECTS)
		$(COMPILE) -o main.bin $(OBJECTS)

main.hex:	main.bin
		rm -f main.hex main.eep.hex
		avr-objcopy -j .text -j .data -O ihex main.bin main.hex
		./checksize main.bin
# do the checksize script as our last action to allow successful compilation
# on Windows with WinAVR where the Unix commands will fail.

disasm:		main.bin
		avr-objdump -d main.bin

cpp:
		$(COMPILE) -E main.c

