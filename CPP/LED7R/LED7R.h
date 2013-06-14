#ifndef _LED7R_H_
#define _LED7R_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {

		using namespace GHI::Interfaces;

		class LED7R : public Module {
			static const unsigned char LEDS = 7;

			Socket* socket;
			DigitalOutput* ports[LED7R::LEDS];

			public:
				LED7R(unsigned char socketNumber);

				void turnOnLED(unsigned char led, bool onlyLED = false);
				void turnOffLED(unsigned char led);

				void turnAllOn();
				void turnAllOff();

				void animate(unsigned int switchTime, bool clockwise, bool turnOn, bool remainOn = true);
		};
	}
}

#endif
