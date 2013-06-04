#ifndef _CHARDISPLAY_H_
#define _CHARDISPLAY_H_

#include <Gadgeteering/Gadgeteering.h>

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

			static const byte DISP_ON = 0xC;
			static const byte CLR_DISP = 1;
			static const byte CUR_HOME = 2;
			static const byte SET_CURSOR = 0x80;
	
			void sendNibble(byte b);
			void sendByte(byte b);
			void sendCommand(byte c);

			public:
				CharDisplay(int socketNumber);

				void print(const char* string);
				void print(char character);
				void clear();
				void cursorHome();
				void setCursor(byte row, byte col);
				void setBacklight(bool state);
		};
	}
}

#endif
