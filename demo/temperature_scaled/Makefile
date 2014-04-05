######################################################################
#  demo/temperature_scaled/Makefile
#  Warren Gay ve3wwg	Sat Apr  5 18:06:59 2014
######################################################################

include /opt/teensy3_lib/Makefile.conf


DEFNS	+= -DUSB_SERIAL

all:	temperature_scaled.hex

temperature_scaled.elf: temperature_scaled.o
	$(CXX) temperature_scaled.o $(LDFLAGS) -o temperature_scaled.elf

temperature_scaled.hex: temperature_scaled.elf
	$(OBJSIZE) temperature_scaled.elf
	$(OBJCOPY) -O ihex -R .eeprom temperature_scaled.elf temperature_scaled.hex	

clean:
	rm -f *.o

distclean: clean
	rm -f temperature_scaled.elf temperature_scaled.hex temperature_scaled.d *~


# End demo/temperature_scaled/Makefile
