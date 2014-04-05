/* Temperature Sensor, Simple Scaling, Teensyduino Tutorial #4
   http://www.pjrc.com/teensy/tutorial4.html

   This example code is in the public domain.
*/

#include <Arduino.h>

int
main(int argc,char **argv) {
	float code;
	float celsius;
	float fahrenheit;

	Serial.begin(38400);

	for (;;) {
		code = analogRead(1);
		celsius = 25 + (code - 512) / 11.3;
		fahrenheit = celsius * 1.8 + 32;
		Serial.print("temperature: ");
		Serial.print(celsius);
		Serial.print(" Celsius, ");
		Serial.print(fahrenheit);
		Serial.println(" Fahrenheit");
		delay(1000);
	}

	return 0;
}
