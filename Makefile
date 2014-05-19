######################################################################
#  TOP LEVEL MAKEFILE
######################################################################
include Makefile.conf

all:	lib/libteensy3.a lib/libfibers.a lib/libslip.a lib/libslipser.a

lib/libteensy3.a::
	$(MAKE) -wC ./teensy3 -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR)

lib/libfibers.a::
	$(MAKE) -wC ./teensy3/teensy3_fibers -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR)

lib/libslip.a::
	$(MAKE) -wC ./teensy3/slip -$(MAKEFLAGS) -f Makefile.teensy TOP_DIR=$(TOP_DIR)

lib/libslipser.a::
	$(MAKE) -wC ./teensy3/slipser -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR)

clean::
	$(MAKE) -wC ./teensy3 -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR) clean
	$(MAKE) -wC ./teensy3/slip -$(MAKEFLAGS) -f Makefile.teensy clean TOP_DIR=$(TOP_DIR)
	$(MAKE) -wC ./teensy3/slipser -$(MAKEFLAGS) clean TOP_DIR=$(TOP_DIR)

distclean::
	$(MAKE) -wC ./teensy3 -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR) distclean
	$(MAKE) -wC ./teensy3/slip -$(MAKEFLAGS) -f Makefile.teensy distclean TOP_DIR=$(TOP_DIR)
	$(MAKE) -wC ./teensy3/slipser -$(MAKEFLAGS) distclean TOP_DIR=$(TOP_DIR)

