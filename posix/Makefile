######################################################################
#  Teensy3_lib/posix/Makefile
#  Warren W. Gay VE3WWG  Fri May 16 19:49:34 2014
######################################################################

include Makefile.conf

PROJS	= SLIP

all:	lib/libteensy.a

lib/libteensy.a::
	for proj in $(PROJS) ; do $(MAKE) -wC ./$$proj -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR); done

clean::
	for proj in $(PROJS) ; do $(MAKE) -wC ./$$proj -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR) clean; done

distclean::
	for proj in $(PROJS) ; do $(MAKE) -wC ./$$proj -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR) distclean; done

# End
