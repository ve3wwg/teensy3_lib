/* Temperature Sensor, Raw Numbers, Teensyduino Tutorial #4
   http://www.pjrc.com/teensy/tutorial4.html

   This example code is in the public domain.
*/

#include <Arduino.h>

int val;

int
main(int argc,char **argv) {

	Serial.begin(38400);

	for (;;) {
		val = analogRead(1);
		Serial.print("analog 1 is: ");
		Serial.println(val);
		delay(1000);
	}

	return 0;
}
