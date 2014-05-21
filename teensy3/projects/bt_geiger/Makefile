######################################################################
#  teensy3_lib/teensy3/projects/bt_geiger
#  Warren Gay ve3wwg	Mon May 19 18:02:05 2014
######################################################################

include $(CURDIR)/../../../Makefile.conf

INCL	+= -I$(TOP_DIR)/posix/bt_geiger

DEFNS	+= -DUSB_SERIAL

all:	bt_geiger.hex

bt_geiger.elf: bt_geiger.o 
	$(CXX) bt_geiger.o $(LDFLAGS) -o bt_geiger.elf -lslipser -lslip -lfibers

bt_geiger.hex: bt_geiger.elf
	$(OBJSIZE) bt_geiger.elf
	$(OBJCOPY) -O ihex -R .eeprom bt_geiger.elf bt_geiger.hex	
	cp bt_geiger.hex ~/.

clean:
	rm -f *.o

distclean: clean
	rm -f bt_geiger.elf bt_geiger.hex *.d errs.t
