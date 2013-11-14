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

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "../Gadgeteering.h"

namespace gadgeteering {
	namespace modules {
		using namespace gadgeteering::interfaces;

		class Button
		{
			digital_input* input;
			digital_output* led;
			bool ledState;

			public:
				Button(unsigned char socketNumber);
				~Button();

				bool isPressed();
				void turnLEDOn();
				void turnLEDOff();
				void toggleLED();
				bool isLEDOn();
		};
	}
}

#endif
