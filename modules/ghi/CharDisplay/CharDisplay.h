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

#ifndef _CHARDISPLAY_H_
#define _CHARDISPLAY_H_

#include "../../../core/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class CharDisplay : public Module {
			Socket* socket;

			DigitalOutput* lcdRS;
			DigitalOutput* lcdE;

			DigitalOutput* lcdD4;
			DigitalOutput* lcdD5;
			DigitalOutput* lcdD6;
			DigitalOutput* lcdD7;

			DigitalOutput* backlight;

			static const unsigned char DISP_ON = 0xC;
			static const unsigned char CLR_DISP = 1;
			static const unsigned char CUR_HOME = 2;
			static const unsigned char SET_CURSOR = 0x80;

			void sendNibble(unsigned char b);
			void sendByte(unsigned char b);
			void sendCommand(unsigned char c);

			public:
				CharDisplay(unsigned char socketNumber);
				~CharDisplay();

				void print(const char* string);
				void print(char character);
				void clear();
				void cursorHome();
				void setCursor(unsigned char row, unsigned char col);
				void setBacklight(bool state);
		};
	}
}

#endif
