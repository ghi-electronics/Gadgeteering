#ifndef _LED7R_H_
#define _LED7R_H_

#include "..\GHI\GHI.h"

class LED7R : public Module {
	static const int LEDS = 7;

	Socket* socket;
	DigitalOutput* ports[LED7R::LEDS];

	public:
		LED7R(int socketNumber);

		void turnOnLED(int led, bool onlyLED = false);
		void turnOffLED(int led);

		void turnAllOn();
		void turnAllOff();
};

#endif
