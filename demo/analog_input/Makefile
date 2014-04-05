######################################################################
#  demo/analog_input/Makefile
#  Warren Gay ve3wwg	Sat Apr  5 18:06:59 2014
######################################################################

include /opt/teensy3_lib/Makefile.conf


DEFNS	+= -DUSB_SERIAL

all:	analog_input.hex

analog_input.elf: analog_input.o
	$(CXX) analog_input.o $(LDFLAGS) -o analog_input.elf

analog_input.hex: analog_input.elf
	$(OBJSIZE) analog_input.elf
	$(OBJCOPY) -O ihex -R .eeprom analog_input.elf analog_input.hex	

clean:
	rm -f *.o

distclean: clean
	rm -f analog_input.elf analog_input.hex analog_input.d *~


# End demo/analog_input/Makefile
