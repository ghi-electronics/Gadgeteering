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

#ifndef _COLORSENSE_H_
#define _COLORSENSE_H_

#include "../Gadgeteering.h"

namespace gadgeteering {
	namespace modules {
		using namespace gadgeteering::interfaces;

		class ColorSense {
			static const unsigned char COLOR_ADDRESS = 0x39;

			digital_output* LEDControl;
			I2CDevice* softwareI2C;

			public:
				struct ColorChannels
				{
					unsigned int Green;
					unsigned int Red;
					unsigned int Blue;
					unsigned int Clear;
				};

				ColorSense(unsigned char socketNumber);
				~ColorSense();

				void ToggleOnboardLED(bool LEDState);
				ColorChannels ReadColorChannels();
		};
	}
}

#endif
