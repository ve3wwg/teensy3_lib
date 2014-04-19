######################################################################
#  teensy3_lib/teensy3/fibers/Makefile
#  Warren Gay ve3wwg	Sat Apr 19 10:21:22 2014
######################################################################

include $(CURDIR)/../../Makefile.conf

C_FILES 	:= $(wildcard *.c)
CPP_FILES 	:= $(wildcard *.cpp)
ASM_FILES	:= $(wildcard *.s)
OBJS 		:= $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o) $(ASM_FILES:.s=.o)

all:	$(LIB_DIR)/libfibers.a

$(LIB_DIR)/libfibers.a: $(OBJS)
	@rm -f $(LIB_DIR)/libfibers.a
	$(AR) cr $(LIB_DIR)/libfibers.a $(OBJS)

clean:
	rm -f *.o

distclean: clean
	rm -f libfibers.a $(LIB_DIR)/libfibers.a *.d

# End teensy3/Makefile
