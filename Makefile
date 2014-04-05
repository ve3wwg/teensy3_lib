######################################################################
#  TOP LEVEL MAKEFILE
######################################################################
include Makefile.conf

all:	lib/libteensy3.a

lib/libteensy3.a::
	$(MAKE) -wC ./teensy3 -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR)

clean::
	$(MAKE) -wC ./teensy3 -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR) clean

distclean::
	$(MAKE) -wC ./teensy3 -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR) distclean
