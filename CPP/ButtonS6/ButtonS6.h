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

#ifndef _LOAD_H_
#define _LOAD_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class ButtonS6 : public Module {
			DigitalInput* buttons[6];
			DigitalOutput* led;

			bool ledState;

			public:
				typedef unsigned char Button;

				class Buttons {
					public:
						static const Button LEFT = 1;
						static const Button RIGHT = 5;
						static const Button UP = 3;
						static const Button DOWN = 4;
						static const Button LEFT_ARROW = 2;
						static const Button RIGHT_ARROW = 6;
				};

				ButtonS6(unsigned char socketNumber);
				~ButtonS6();

				bool isPressed(Button buttonNumber);
				void turnLEDOn();
				void turnLEDOff();
				void toggleLED();
				bool isLEDOn();
		};
	}
}

#endif
