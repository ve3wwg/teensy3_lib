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

#define SZ_CURRENT_TIME		1+6

struct s_current_time {
	uint8_t		pkttype;
	uint8_t		year;		// Years since 2014
	uint8_t		month;		// Month 1-12
	uint8_t		mday;		// Day of month 1-31
	uint8_t		hour;		// 0-23
	uint8_t		min;		// 0-59
	uint8_t		sec;		// 0-59
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
