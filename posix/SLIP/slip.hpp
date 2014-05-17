//////////////////////////////////////////////////////////////////////
// slip.hpp -- SLIP Protocol Class
// Date: Fri May 16 19:54:36 2014  Warren Gay ve3wwg
///////////////////////////////////////////////////////////////////////
// SLIP protocol object :
//
// This is useful for "framing" serial binary messages between the
// Arduino and the PC (possibly over USB), for example.
//
// ===================================================================
//
// The user defined read routine must return 1 when a byte is returned.
// A zero indicates EOF (useful when debugging with files), but may
// not apply to the Arduino code. The read routine should return -1
// if an I/O error occurred (no data was returned).
//
// The user defined write routine should return zero when successful,
// but otherwise return -1 if an error occurred.
//
// SLIP::recv_packet() returns the packet length received.  It
// will return -2 if the packet was truncated (too long for the
// supplied buffer). -1 is returned if an I/O error was encountered.
//
// SLIP::send_packet() returns zero when sucessful, else -1 indicates
// an I/O error occurred.
//
// The user_data parameter should be set to zero if not used. 
// It is always passed to the read_byte() and write_byte() routines
// and may point to anything the user routines find useful.
//
// ===================================================================

#ifndef SLIP_HPP
#define SLIP_HPP

#include <stdint.h>

extern "C" {
	typedef uint8_t (*slipread_t)();
	typedef void (*slipwrite_t)(uint8_t b);
}

class SLIP {
	slipread_t		read_b;		// Routine to fetch data with
	slipwrite_t		write_b;	// Routine to write data with
	bool			use_crc8;	// Include CRC8 in packet

	void write_encoded(uint8_t b);		// Write encoded byte

public:	enum Status {
		ES_Ok = 0,			// Success
		ES_Trunc,			// Packet was too long for buffer (truncated)
		ES_Protocol,			// Protocol error
		ES_CRC				// CRC Failure
	};

	SLIP(slipread_t read_func,slipwrite_t write_func);
	bool enable_crc8(bool on);
	void write(const void *buffer,unsigned length);
	Status read(void *buffer,unsigned buflen,unsigned& retlength);
};

#endif // SLIP_HPP

// End slip.hpp
