######################################################################
#  teensy3_lib/teensy3/projects/Makefile
#  Warren Gay ve3wwg	Sat Apr  5 08:47:58 2014
######################################################################

include $(CURDIR)/../../Makefile.conf

PROJS	= bt_geiger

all::
	for p in $(PROJS) ; do $(MAKE) -wC $(CURDIR)/$$p -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR) ; done

clean:
	for p in $(PROJS) ; do $(MAKE) -wC $(CURDIR)/$$p -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR) clean ; done

distclean: clean
	for p in $(PROJS) ; do $(MAKE) -wC $(CURDIR)/$$p -$(MAKEFLAGS) TOP_DIR=$(TOP_DIR) distclean ; done

# End Makefile
