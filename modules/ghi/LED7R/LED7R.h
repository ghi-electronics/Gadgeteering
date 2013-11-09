/*
Copyright 2013 GHI Electronics LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _LED7R_H_
#define _LED7R_H_

#include <core/Gadgeteering.h>

namespace GHI {
	namespace Modules {

		using namespace GHI::Interfaces;

		class LED7R : public Module {
			static const unsigned char LEDS = 7;

			Socket* socket;
			DigitalOutput* ports[LED7R::LEDS];

			public:
				LED7R(unsigned char socketNumber);
				~LED7R();

				void turnOnLED(unsigned char led, bool onlyLED = false);
				void turnOffLED(unsigned char led);

				void turnAllOn();
				void turnAllOff();

				void set(unsigned char led, bool state);

				void animate(unsigned int switchTime, bool clockwise, bool turnOn, bool remainOn = true);
		};
	}
}

#endif
