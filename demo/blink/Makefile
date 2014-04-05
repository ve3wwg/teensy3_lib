######################################################################
#  test2/Makefile
#  Warren Gay ve3wwg	Sat Apr  5 09:53:41 2014
######################################################################

include /opt/teensy3_lib/Makefile.conf

all:	blink.hex

blink.elf: blink.o
	$(CXX) blink.o $(LDFLAGS) -o blink.elf

blink.hex: blink.elf
	$(OBJSIZE) blink.elf
	$(OBJCOPY) -O ihex -R .eeprom blink.elf blink.hex	

clean:
	rm -f *.o

distclean: clean
	rm -f blink.elf blink.hex blink.d *~
