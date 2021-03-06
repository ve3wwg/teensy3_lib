teensy3_lib
===========

Teensy 3.1 development environment, without use of the Arduino IDE


This library assumes that you've installed a suitable compiler suite from
launchpad.net/gcc-arm-embedded or an equivalent (perhaps you built your
own cross-compiler toolchain).  This does not cover Windows installs.

Make sure that you have your toolchain in your PATH, before you perform
a "make".  I usually use an alias to do this for me:

    alias use_arm='PATH="/opt/gcc-arm/bin:$PATH"'

Then when I want to do some teensy3 development, the simple alias command
sets up my PATH accordingly:

$ use_arm


OSX/Linux INSTALL:
==================

    1. Install the teensy3_lib directory to /opt/teensy3_lib.
    2. cd /opt/teensy3_lib
    3. sudo make

If you need to re-make after changes, do:

    1. make distclean
    2. make clean


INSTALLING TO ANOTHER DIRECTORY:
================================

If installing this library to another location, like perhaps
/usr/local/share/teensy3_lib, then:

    1. Edit the file teensy3_lib/Makefile.conf
    2. Change the value of the macro TOP_DIR to read:

    TOP_DIR=/usr/local/share/teensy3_lib

    3. cd /usr/local/share/teensy3_lib
    4. sudo make distclean (if necessary)
    5. sudo make


CUSTOM CONFIGURATIONS:
======================

If you need to customize other aspects of your build, see the file
/opt/teensy3_lib/Makefile.conf:

F_CPU: For example, you can change the  definition of -DF_CPU there (or
just override the value in your own Makefile).

LDSCRIPT: To configure for the teensy3.0, alter the macro LDSCRIPT to
point to the mk20dx128.ld (or mk20dx128_sbrk.ld).

TOOLCHAIN: Changing the value of ARCH allows a different command name
prefix to be used (it defaults to arm-none-eabi-).


YOUR APPLICATION MAKEFILE:
==========================

    include /opt/teensy3_lib/Makefile.conf
    
    all:	main.hex
    
    main.elf: main.o
    	$(CXX) main.o $(LDFLAGS) -o main.elf
    
    main.hex: main.elf
    	$(OBJSIZE) main.elf
    	$(OBJCOPY) -O ihex -R .eeprom main.elf main.hex	
    
    clean:
    	rm -f *.o
    
    clobber: clean
    	rm -f main main.hex


YOUR BUILD:
===========

    1. make
    2. make clean     (cleans object files)
    3. make clobber   (cleans object files and final executables)


MAIN PROGRAM:
=============

Note that your program will use a real "main" program - NOT
the Arduino setup() or loop() functions. If you wish to
use the Arduino setup() and loop(), remove the macro
definition -DNON_ARDUINO from /opt/teensy3_lib/Makefile.conf .

    #include <Arduino.h>
    #include <usb_api.h>
    #include <usb_serial.h>
    
    #include <stdlib.h>
    
    usb_serial_class serial;
    
    int
    main(int argc,char **argv) {
    
            serial.begin(9600);
    
            serial.write("Hello World!!!\n",18);
            serial.send_now();
    
            delay(1500);
    
            serial.write("Bye.\n",8);
            serial.send_now();
    
            delay(1000);
    
            serial.end();
            return 0;
    }


TESTING:
========

See teensy3_lib/demo/blink:


DEMO PROGRAMS:
==============

Arduino demo programs that compile for the teensy3, are found in the
teensy3_lib/demo subdirectory. The top-level teensy3_lib/demo/Makefile
will build all demo programs by typing "make". 

Otherwise cd to the project directory of interest, and type "make" to
build the project.

To clean up, use "make distclean".


--
Warren Gay ve3wwg@gmail.com
