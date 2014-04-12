######################################################################
#  teensy3_lib/teensy3/Makefile
#  Warren Gay ve3wwg	Sat Apr  5 08:47:58 2014
######################################################################

include $(CURDIR)/../Makefile.conf

DEFNS	+= -DUSB_SERIAL

C_FILES 	:= $(wildcard *.c)
CPP_FILES 	:= $(wildcard *.cpp)
OBJS 		:= $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)

all:	$(LIB_DIR)/libteensy3.a

$(LIB_DIR)/libteensy3.a: $(OBJS)
	@rm -f $(LIB_DIR)/libteensy3.a
	$(AR) cr $(LIB_DIR)/libteensy3.a $(OBJS)

clean:
	rm -f *.o

distclean: clean
	rm -f libteensy3.a $(LIB_DIR)/libteensy3.a

# End teensy3/Makefile
