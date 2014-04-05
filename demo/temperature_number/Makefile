######################################################################
#  demo/temperature_number/Makefile
#  Warren Gay ve3wwg	Sat Apr  5 18:06:59 2014
######################################################################

include /opt/teensy3_lib/Makefile.conf


DEFNS	+= -DUSB_SERIAL

all:	temperature_number.hex

temperature_number.elf: temperature_number.o
	$(CXX) temperature_number.o $(LDFLAGS) -o temperature_number.elf

temperature_number.hex: temperature_number.elf
	$(OBJSIZE) temperature_number.elf
	$(OBJCOPY) -O ihex -R .eeprom temperature_number.elf temperature_number.hex	

clean:
	rm -f *.o

distclean: clean
	rm -f temperature_number.elf temperature_number.hex temperature_number.d *~


# End demo/temperature_number/Makefile
