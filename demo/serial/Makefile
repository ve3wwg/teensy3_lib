######################################################################
#  demo/serial/Makefile
#  Warren Gay ve3wwg	Sat Apr  5 17:17:10 2014
######################################################################

include /opt/teensy3_lib/Makefile.conf


DEFNS	+= -DUSB_SERIAL

all:	serial.hex

serial.elf: serial.o
	$(CXX) serial.o $(LDFLAGS) -o serial.elf

serial.hex: serial.elf
	$(OBJSIZE) serial.elf
	$(OBJCOPY) -O ihex -R .eeprom serial.elf serial.hex	

clean:
	rm -f *.o

distclean: clean
	rm -f serial.elf serial.hex serial.d


# End demo/serial/Makefile
