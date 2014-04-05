######################################################################
#  demo/analog_rgb_color/Makefile
#  Warren Gay ve3wwg	Sat Apr  5 18:06:59 2014
######################################################################

include /opt/teensy3_lib/Makefile.conf


DEFNS	+= -DUSB_SERIAL

all:	analog_rgb_color.hex

analog_rgb_color.elf: analog_rgb_color.o
	$(CXX) analog_rgb_color.o $(LDFLAGS) -o analog_rgb_color.elf

analog_rgb_color.hex: analog_rgb_color.elf
	$(OBJSIZE) analog_rgb_color.elf
	$(OBJCOPY) -O ihex -R .eeprom analog_rgb_color.elf analog_rgb_color.hex	

clean:
	rm -f *.o

distclean: clean
	rm -f analog_rgb_color.elf analog_rgb_color.hex analog_rgb_color.d *~


# End demo/analog_rgb_color/Makefile
