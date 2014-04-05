# demo/Makefile

SUBDIRS = hello_world usb_to_serial


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


