#ifndef _CHARDISPLAY_H_
#define _CHARDISPLAY_H_

#include "../Gadgeteering/Gadgeteering.h"

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
