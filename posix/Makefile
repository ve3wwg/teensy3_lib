######################################################################
#  Teensy3_lib/posix/Makefile
#  Warren W. Gay VE3WWG  Fri May 16 19:49:34 2014
######################################################################

include Makefile.conf

SHPROJS	= slip sqlite3db
PROJS	=

all:	lib/libteensy.a

lib/libteensy.a::
	for proj in $(SHPROJS) ; do $(MAKE) -wC ./$$proj -$(MAKEFLAGS) -f Makefile.posix TOP_DIR=$(TOP_DIR); done
	for proj in $(PROJS) ; do $(MAKE) -wC ./$$proj -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR); done

clean::
	for proj in $(SHPROJS) ; do $(MAKE) -wC ./$$proj -$(MAKEFLAGS) -f Makefile.posix clean TOP_DIR=$(TOP_DIR); done
	for proj in $(PROJS) ; do $(MAKE) -wC ./$$proj -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR) clean; done

distclean::
	for proj in $(SHPROJS) ; do $(MAKE) -wC ./$$proj -$(MAKEFLAGS) -f Makefile.posix distclean TOP_DIR=$(TOP_DIR); done
	for proj in $(PROJS) ; do $(MAKE) -wC ./$$proj -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR) distclean; done

# End
