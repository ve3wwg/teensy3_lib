# demo/Makefile

SUBDIRS	= analog_input analog_rgb_color blink serial temperature_number temperature_scaled temperature_scaled_multi usb_serial


all:
	for subdir in $(SUBDIRS) ; do \
		$(MAKE) -wC $$subdir -$(MAKEFLAGS) ; \
	done

clean:
	for subdir in $(SUBDIRS) ; do \
		$(MAKE) -wC $$subdir -$(MAKEFLAGS) clean ; \
	done

distclean:
	for subdir in $(SUBDIRS) ; do \
		$(MAKE) -wC $$subdir -$(MAKEFLAGS) distclean ; \
	done

