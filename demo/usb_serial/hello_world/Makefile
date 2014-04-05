######################################################################
#  demo/usb_serial/hello_world/Makefile
#  Warren Gay ve3wwg	Sat Apr  5 17:22:38 2014
######################################################################

include /opt/teensy3_lib/Makefile.conf

DEFNS	+= -DUSB_SERIAL

all:	hello_world.hex

hello_world.elf: hello_world.o
	$(CXX) hello_world.o $(LDFLAGS) -o hello_world.elf

hello_world.hex: hello_world.elf
	$(OBJSIZE) hello_world.elf
	$(OBJCOPY) -O ihex -R .eeprom hello_world.elf hello_world.hex	

clean:
	rm -f *.o

distclean: clean
	rm -f hello_world.elf hello_world.hex hello_world.d


# End demo/usb_serial/hello_world/Makefile
