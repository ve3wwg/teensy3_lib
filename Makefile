######################################################################
#  TOP LEVEL MAKEFILE
######################################################################
include Makefile.conf

all:	lib/libteensy3.a lib/libfibers.a

lib/libteensy3.a::
	$(MAKE) -wC ./teensy3 -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR)

lib/libfibers.a::
	$(MAKE) -wC ./teensy3/teensy3_fibers -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR)

clean::
	$(MAKE) -wC ./teensy3 -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR) clean

distclean::
	$(MAKE) -wC ./teensy3 -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR) distclean
