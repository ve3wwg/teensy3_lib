//////////////////////////////////////////////////////////////////////
// fibers.hpp -- Fibers Class Support
// Date: Sat Apr 19 00:14:27 2014   (C) Warren Gay VE3WWG
///////////////////////////////////////////////////////////////////////
// DESCRIPTION:
// 
// This is a very light weight fibers (aka coroutines) library,
// managed by a singular instance of class Fibers. This module
// depends upon two symbols in your ld script, which is usually
// mk20dx256.ld (or in my case mk20dx256_sbrk.ld).
//
// See the end of this file for more information
///////////////////////////////////////////////////////////////////////

#ifndef FIBERS_HPP
#define FIBERS_HPP

#include <stdint.h>

extern "C" {
	// Your coroutine functions will use this function prototype
	typedef void (*fiber_func_t)(void *arg);
}

//////////////////////////////////////////////////////////////////////
// Increase MAX_FIBERS if you need more than 15 coroutines
//////////////////////////////////////////////////////////////////////

#define MAX_FIBERS		16	        // Includes main thread


//////////////////////////////////////////////////////////////////////
// There must only be ONE instance of this class defined
//////////////////////////////////////////////////////////////////////

class Fibers {
	// Each structure manages the state of one main/coroutine instance
public:	struct fiber_t {
		uint32_t	sp;		// Saved sp register
		uint32_t	r0;		// Arg 1 at function startup, else saved r0
		uint32_t	r2;		//  after the function has begun
		uint32_t	r3;		// Saved r3 etc.
		uint32_t	r4;
		uint32_t	r5;
		uint32_t	r6;
		uint32_t	r7;
		uint32_t	r8;
		uint32_t	r9;
		uint32_t	r10;
		uint32_t	r11;
		uint32_t	r12;
		uint32_t	lr;		// Return address (pc)
		uint32_t	stack_size;	// Stack size for this main/coroutine
	};

protected:
	fiber_t			fibers[MAX_FIBERS];	// Collection of fiber states
	uint16_t		n_fibers;		// Number of active fibers
	uint16_t		cur_crx;		// Index to currently executing coroutine
	bool			instrument;		// Instrument stack for stack size measurement
	uint32_t		pattern;		// Pattern to use on stack for instrumentation

	uint32_t *end_stack();				// Return end of stack address (requires _sstack)

public:	Fibers(uint32_t main_stack=8192,bool instrument=false,uint32_t pattern_override=0xA5A5A5A5);

	unsigned create(fiber_func_t func,void *arg,uint32_t stack_size);

	inline unsigned coroutine() { return cur_crx; }	// Return current coroutine number (0 == main)
	void yield();					// Yield CPU to next coroutine
	inline uint32_t size() { return n_fibers; }	// Return the current # of coroutines

	uint32_t stack_size(uint16_t coroutine);	// Return the approximate stack usage for specified coroutine 
};

#endif // FIBERS_HPP

//////////////////////////////////////////////////////////////////////
// DESCRIPTION:
// 
// This is a very light weight fibers (aka coroutines) library,
// managed by a singular instance of class Fibers. This module
// depends upon two symbols in your ld script, which is usually
// mk20dx256.ld (or in my case mk20dx256_sbrk.ld).
// 
// The two required symbols are:
// 
// 	_estack		End stack: Which should already be defined for you
// 	_sstack		Start stack: Which is probably not defined
// 
// To add the _sstack symbol to your linking, locate your
// mk20dx256.ld (or equivalent) script:
// 
//     $ find <root_dir> -name 'mk20dx256*'
// 
// where <root_dir> varies by platform. Using my teensy3_lib
// structure, the root_dir is likely /opt/teensy3_lib.
// 
// Otherwise, using the Arduino IDE, use
// /Applications/Arduino.app for your root_dir on the Mac. On
// Windows, you'll have to sort that out for yourself.
// 
// Look towards the end if the file for the section labelled .stack :
// 
// 	.stack :
// 	{
// 	    . = ALIGN(4);
//             _sstack = .; /* Add this line to define the start of the stack */
// 	    . = . + _minimum_stack_size;
// 	    . = ALIGN(4);
// 	} >RAM
// 
// 	_test_estack = ORIGIN(RAM) + LENGTH(RAM);
// 	_estack = ORIGIN(RAM) + 64 * 1024;
// 
// The symbol for _estack should already be defined. Add the line
// above for _sstack where the comment says "Add this line", if
// necessary.
// 
// The purpose of the _sstack symbol is to tell the Fibers class
// where the stack begins. This is only required if you are
// "instrumenting" the stack to later find out how much stack
// space was actually used by a given coroutine (or main
// routine). Unfortunately, this symbol is required whether you
// plan to use this feature or not.
// 
// COMPILING:
// ----------
// 
// When using the teensy3_lib structure, the class header should be
// includable in your C++ code as:
// 
// //  #include <fibers.hpp>
// 
// Arduino IDE users may be out of luck, since one assembler routine
// is required (it can be done, but messing with the IDE build would
// be necessary to include that one assembled object file).
// 
// LINKING:
// --------
// 
// When using the teensy3_lib structure, the fibers library just needs
// the linker option added:
// 
//     -lfibers
// 
// Fibers API Usage:
// -----------------
// 
// Somewhere it is recommended that you declare one instance of
// Fibers globally. Global access makes it available from other
// modules when it is time to "yield".
// 
//     Fibers fibers;              // Uses a default main stack size of 8K
// or
//     Fibers fibers(3000,true);   // Use a main stack of 3000 bytes, and
//                                 // instrument stack for size checks    
// 
// Once this has been done, stack space has been reserved for the
// main thread.
// 
// To create a coroutine, you invoke the Fibers::create() method:
// 
//     fibers.create(foo,foo_arg,4000);
// 
// It returns an unsigned number 1-n, for identification if you
// need it. This example will start function:
// 
//     void foo(void *foo_arg)
// 
// with an allocated stack of 4000 bytes.
// 
// Additional coroutines can be created from any executing 
// coroutine context.
// 
// From this point on, the main routine or the executing coroutine
// (in foo) call the yield method to give up the CPU:
// 
//     fibers.yield();
// 
// Scheduling is done in a simple round-robin fashion, giving
// the CPU to the next coroutine in sequence.
// 
// COROUTINES THAT "RETURN":
// =========================
// 
// If a coroutine (other than main) "returns", it will simply
// start to execute again. Note that after restarting, the
// coroutine's argument "arg" will likely be trash, and should
// not be used again. It will simply represent the last saved
// value for register "r0".
// 
// A flag should be set in your coroutine the first time it
// runs, so that it may make this important distinction at
// startup.
// 
// DETERMINING STACK SIZE:
// =======================
// 
// The first question that arises when creating threads or fibers
// is "how much stack space do I need?" To estimate this, the
// Fibers class has the optional "instrument" boolean, which
// when enabled, causes the stack to be initialized with a "pattern".
// By default this feature is disabled (see constructor). When
// enabled the default pattern used is 0xA5A5A5A5.
// 
// Allow your coroutines to run for a time and then you can:
// 
//     bytes = fibers.stack_size(x);
// 
// where x is the coroutine number (x==0 is the main routine).
// 
// This estimates usage based upon the stack pattern words being
// overwritten with something other than "pattern". Even if you
// don't use any coroutines, you should be able to estimate the
// stack use of the main program.
// 
// If you suspect a false match on "pattern" has occurred,
// causing a smaller size to be returned, repeat the test using a
// different uint32_t pattern value, that is unlikely to appear
// on the stack.
//
// WARNING:
// ========
// When allocating stack space, be sure to include an extra
// amount for use by interrupt service routines.
//////////////////////////////////////////////////////////////////////

// End fibers.hpp
