#ifndef _LEDSTRIP_H_
#define _LEDSTRIP_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {

		using namespace GHI::Interfaces;

		class LEDStrip : public Module {
			static const unsigned char LEDS = 7;

			Socket* socket;
			DigitalOutput* ports[LEDStrip::LEDS];

			public:
				LEDStrip(unsigned char socketNumber);
				virtual ~LEDStrip();

				void turnOnLED(unsigned char led, bool onlyLED = false);
				void turnOffLED(unsigned char led);

				void turnAllOn();
				void turnAllOff();

				void animate(unsigned int switchTime, bool clockwise, bool turnOn, bool remainOn = true);
		};
	}
}

#endif
