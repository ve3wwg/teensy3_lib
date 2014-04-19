//////////////////////////////////////////////////////////////////////
// fibers.cpp -- Fibers Class Implementation
// Date: Sat Apr 19 00:21:32 2014
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "fibers.hpp"

//////////////////////////////////////////////////////////////////////
// Assembler routines used:
//////////////////////////////////////////////////////////////////////

extern "C" {
	extern uint32_t *fiber_getsp();
	extern void fiber_set_main(uint32_t stack_size);
	extern void fiber_create(Fibers::fiber_t *fcb,uint32_t stack_size,fiber_func_t func,void *arg);
	extern void fiber_swap(Fibers::fiber_t *new_cr,Fibers::fiber_t *cur_cr);
}

//////////////////////////////////////////////////////////////////////
// Constructor for Fibers:
//
//	main_stack	Indicates # of stack bytes to reserve for main
//	instrument	When true, the stack is cleared with a pattern
//			so that stack_size() can determine stack
//			usage. (Default false)
//	pattern_override Default: 0xA5A5A5A5, but can be overriden by
//			the user, if false matches have been a problem
//////////////////////////////////////////////////////////////////////

Fibers::Fibers(uint32_t main_stack,bool instrument,uint32_t pattern_override) {

	pattern = pattern_override;	// Fill pattern for stack (when enabled)

	fiber_set_main(main_stack);	// Set stack size needed by the main coroutine

	n_fibers = 1;			// Reserve fibers[0] for main thread
	cur_crx = 0;			// Main is currently in control
	fibers[0].stack_size = main_stack; // Save main's stack size

	this->instrument = instrument;	// Enable/disable instrumentation

	if ( instrument ) {
		volatile uint32_t *ep = end_stack();
		uint32_t *sp = fiber_getsp();

		while ( --sp > ep )
			*sp = pattern;	// Fill stack with pattern
	}
}

//////////////////////////////////////////////////////////////////////
// Create a new fiber:
//
// RETURNS:
//	1-n for each fiber (coroutine) created
//	~0 if we exceeded the maximum number of fibers
//
// NOTES:
//	1. Can be called from any fiber (aka coroutine)
//	2. Returns a value 1-n, for nth coroutine
//	3. Zero represents the main fiber (coroutine)
//	4. This code does not check if there is enough stack space
//	   for the request. Choose stack size carefully.
//////////////////////////////////////////////////////////////////////

unsigned
Fibers::create(fiber_func_t func,void *arg,uint32_t stack_size) {

	// Round stack size to a word multiple
	stack_size = ( stack_size + sizeof (uint32_t) ) / sizeof (uint32_t) * sizeof (uint32_t);

	if ( n_fibers >= MAX_FIBERS )
		return 0;		// Failed: too many fibers

	fiber_t &new_cr = fibers[n_fibers++];

	fiber_create(&new_cr,stack_size,func,arg);
	new_cr.stack_size = stack_size;

	return n_fibers - 1;		// Return zero-based coroutine # (main == 0)
}

//////////////////////////////////////////////////////////////////////
// Yield CPU to next coroutine, round-robin 
//
// NOTES:
//	1. Normally, this causes control to pass to the next fiber,
//	   in round-robin fashion, returning to the caller after we
// 	   get the CPU passed back to us again.
//	2. Except when there is only the main context running, which
//	   then simply returns immediately to the caller again.
//////////////////////////////////////////////////////////////////////

void
Fibers::yield() {

	if ( n_fibers <= 1 )
		return;			// There is only the main context running

	uint16_t nextx = cur_crx + 1;	// Compute the coroutine # of the next fiber
	if ( nextx >= n_fibers )
		nextx = 0;		// Wrap around to main context at the end

	fiber_t& last_cr = fibers[cur_crx];	// Current context
	fiber_t& next_cr = fibers[nextx];	// Next context
	cur_crx = nextx;			// Change the context no.

	fiber_swap(&next_cr,&last_cr);		// Make the switch

	// cur_crx has been restored by the last yield() caller
}

//////////////////////////////////////////////////////////////////////
// Determine the coroutine's stack size in bytes
//
//	1. Must have been instrumented (see constructor)
//	2. Returns approx size in bytes
//	3. Determined on a 'best effort' basis, since a false
//	   match on the instrumented pattern may cause a smaller
//	   size to be returned.
//////////////////////////////////////////////////////////////////////

uint32_t
Fibers::stack_size(uint16_t coroutine) {

	if ( !instrument )				// Not instrumented
		return ~0;				// .. so size unknown

	if ( coroutine >= n_fibers )
		return ~0;				// Invalid fiber/coroutine #

	fiber_t& crentry = fibers[coroutine];		// Access requested coroutine
	uint32_t *sp = (uint32_t *)crentry.sp;		// Get it's sp
	uint32_t *ep = sp - crentry.stack_size/4;	// stack_size has been rounded to a word multiple
	uint32_t count = 0;				// Initialize count

	for ( ; sp >= ep && *sp != pattern; --sp )
		count += 4;

	return count;					// Bytes
}

//////////////////////////////////////////////////////////////////////
// Protected: Return address of end of stack
//////////////////////////////////////////////////////////////////////

uint32_t *
Fibers::end_stack() {
	extern uint32_t _sstack;			// This comes from mk20dx256*.ld
							// See fibers.hpp at end.
	return &_sstack;
}

// End fibers.cpp
