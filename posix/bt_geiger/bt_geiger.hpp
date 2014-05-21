//////////////////////////////////////////////////////////////////////
// bt_geiger.hpp -- bt_geiger application header
// Date: Sun May 18 18:03:11 2014   (C) Warren Gay ve3wwg
///////////////////////////////////////////////////////////////////////

#ifndef BT_GEIGER_HPP
#define BT_GEIGER_HPP

#define UNKNOWN_BT 		"/dev/cu.Bluetooth"
#define KEY_BLUETOOTH_DEV	"bluetooth_dev"
#define KEY_BLUETOOTH_SPEED	"bluetooth_speed"

enum PacketType {
	PT_CurrentTime = 7,		// s_current_time
};

//////////////////////////////////////////////////////////////////////
// Used to inform Teensy what the current date/time is
//////////////////////////////////////////////////////////////////////

#define SZ_CURRENT_TIME		1+4

struct s_current_time {
	uint8_t		pkttype;
	uint32_t	time;		// Current time_t value
} __attribute__((packed));

//////////////////////////////////////////////////////////////////////
// Union of all supported packet types
//////////////////////////////////////////////////////////////////////

union u_packets {
	uint8_t		pkt_type;
	s_current_time	pkt_curtime;
} __attribute__((packed));

#endif // BT_GEIGER_HPP

// End bt_geiger.hpp
