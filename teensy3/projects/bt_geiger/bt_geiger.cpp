//////////////////////////////////////////////////////////////////////
// bt_geiger.cpp -- Teensy 3.1 Geiger Monitor Project
// Date: Mon May 19 17:53:09 2014
///////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <stdint.h>
#include <fibers.h>

#define TEENSY
#include "bt_geiger.hpp"
#include "slipser.hpp"

static Fibers<4> fibers(4096);
static SlipSer bluetooth(Serial2);
static int led = 13;
static char rxbuf[1024];

static void
send_current_time() {
	unsigned long t = Teensy3Clock.get();
	s_current_time msg;

	msg.pkttype = PT_CurrentTime;
	msg.time = static_cast<uint32_t>(t);
	bluetooth.write(&msg,sizeof msg);
}

static void
rx_thread(void *) {
	u_packets *packet = 0;
	unsigned pktlen = 0;
	int led_state = 1;
	
	for (;;) {
		packet = static_cast<u_packets *>(bluetooth.read(pktlen));
		led_state ^= 1;
		digitalWrite(led,led_state ? HIGH : LOW);
	
		switch ( packet->pkt_type ) {
		case PT_CurrentTime :
			if ( pktlen == SZ_CURRENT_TIME ) {
				time_t rtc = Teensy3Clock.get();
				time_t mac = packet->pkt_curtime.time;
			
				if ( rtc != mac )
					Teensy3Clock.set(mac);		// Adjust our realtime clock
			}
			break;
		default :
			;
		}
		yield();
	}
}

extern "C" {
	void yield() {
		fibers.yield();
	}
}

int
main() {

	pinMode(led,OUTPUT);
	digitalWrite(led,HIGH);

	bluetooth.open(38400,rxbuf,sizeof rxbuf);

	fibers.create(rx_thread,0,4096);

	for (;;) {
		send_current_time();
		delay(1000);
		fibers.yield();
	}
	
	return 0;
}

// End bt_geiger.cpp
