######################################################################
#  demo/usb_serial/usb_to_serial/Makefile
#  Warren Gay ve3wwg	Sat Apr  5 17:24:57 2014
######################################################################

include /opt/teensy3_lib/Makefile.conf


DEFNS	+= -DUSB_SERIAL

all:	usb_to_serial.hex

usb_to_serial.elf: usb_to_serial.o
	$(CXX) usb_to_serial.o $(LDFLAGS) -o usb_to_serial.elf

usb_to_serial.hex: usb_to_serial.elf
	$(OBJSIZE) usb_to_serial.elf
	$(OBJCOPY) -O ihex -R .eeprom usb_to_serial.elf usb_to_serial.hex	

clean:
	rm -f *.o

distclean: clean
	rm -f usb_to_serial.elf usb_to_serial.hex usb_to_serial.d


# End demo/usb_serial/usb_to_serial/Makefile
