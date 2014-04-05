// LED Blink, Teensyduino Tutorial #1
//   http://www.pjrc.com/teensy/tutorial.html
// 
//   This example code is in the public domain.

#include <Arduino.h>

// Teensy 3.0 has the LED on pin 13

static const int ledPin = 13;

int
main(int argc,char **argv) {

	// initialize the digital pin as an output.
	pinMode(ledPin, OUTPUT);

	for (;;) {
		digitalWrite(ledPin,HIGH);   // set the LED on
		delay(1000);                  // wait for a second
		digitalWrite(ledPin,LOW);    // set the LED off
		delay(1000);                  // wait for a second
	}
}

