######################################################################
#  demo/temperature_scaled_multi/Makefile
#  Warren Gay ve3wwg	Sat Apr  5 18:06:59 2014
######################################################################

include /opt/teensy3_lib/Makefile.conf


DEFNS	+= -DUSB_SERIAL

all:	temperature_scaled_multi.hex

temperature_scaled_multi.elf: temperature_scaled_multi.o
	$(CXX) temperature_scaled_multi.o $(LDFLAGS) -o temperature_scaled_multi.elf

temperature_scaled_multi.hex: temperature_scaled_multi.elf
	$(OBJSIZE) temperature_scaled_multi.elf
	$(OBJCOPY) -O ihex -R .eeprom temperature_scaled_multi.elf temperature_scaled_multi.hex	

clean:
	rm -f *.o

distclean: clean
	rm -f temperature_scaled_multi.elf temperature_scaled_multi.hex temperature_scaled_multi.d *~


# End demo/temperature_scaled_multi/Makefile
