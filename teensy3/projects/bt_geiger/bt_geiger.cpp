//////////////////////////////////////////////////////////////////////
// bt_geiger.cpp -- Teensy 3.1 Geiger Monitor Project
// Date: Mon May 19 17:53:09 2014
///////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <stdint.h>

#define TEENSY
#include "bt_geiger.hpp"
#include "slipser.hpp"

static SlipSer bluetooth(Serial2);

static char rxbuf[1024];

static void
send_current_time() {
	static uint8_t sec = 0;
	s_current_time msg;

	msg.pkttype = PT_CurrentTime;
	msg.year = 0;
	msg.month = 5;
	msg.mday = 19;
	msg.hour = 17;
	msg.min = 0;
	msg.sec = sec++;

	bluetooth.write(&msg,sizeof msg);
}

int
main() {
	int led = 13;

	pinMode(led,OUTPUT);
	digitalWrite(led,HIGH);

	bluetooth.open(38400,rxbuf,sizeof rxbuf);

	int x = 0;

	for (;;) {
		digitalWrite(led,x?LOW:HIGH);
		send_current_time();
		delay(1000);
	}
	
	return 0;
}

// End bt_geiger.cpp
