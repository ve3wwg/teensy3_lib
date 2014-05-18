######################################################################
#  teensy3_lib/posix/bt_geiger/Makefile
#  Warren Gay ve3wwg	Sun May 18 16:14:41 2014
######################################################################

include ../Makefile.conf

C_FILES 	:= $(wildcard *.c)
CPP_FILES 	:= $(wildcard *.cpp)
OBJS 		:= $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)

all:	bt_geiger bt_dummy

bt_geiger: bt_geiger.o
	$(CXX) bt_geiger.o -o bt_geiger $(LDFLAGS) -lteensy -lsqlite3

bt_dummy: bt_dummy.o
	$(CXX) bt_dummy.o -o bt_dummy $(LDFLAGS) -lteensy

check:

clean:
	rm -f *.o bt_geiger

distclean: clean
	rm -f bt_geiger bt_dummy errs.t bt_geiger.db

# End posix/sqlite3/Makefile
