#ifndef _DISPLAYN18_H_
#define _DISPLAYN18_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class DisplayN18 : public Module {
			static const unsigned char STEP_X = 16;
			static const unsigned char STEP_Y = 20;
			static const unsigned int WIDTH = 128;
			static const unsigned int HEIGHT = 160;

			DigitalOutput* resetPin;
			DigitalOutput* backlightPin;
			DigitalOutput* rsPin;
			SPIConfiguration* config;
			SPIDevice* spi;
			
			void writeCommand(unsigned char command);
			void writeData(unsigned char data);
			void writeData(const unsigned char* data, unsigned int length);

			void reset();
			void initialize();
			void setClippingArea(unsigned char x, unsigned char y, unsigned char width, unsigned char height);

			public:
				DisplayN18(unsigned char socketNumber);
				~DisplayN18();
				
				static unsigned short rgbToShort(unsigned char r, unsigned char g, unsigned char b);
				void clearScreen(unsigned short color = 0x0000);
				void fillRect(unsigned char x, unsigned char y, unsigned char width, unsigned char height, unsigned short color);
				void setPixel(unsigned char x, unsigned char y, unsigned short color);
				void drawRaw(const unsigned char* data, unsigned char x, unsigned char y, unsigned char width, unsigned char height);
		};
	}
}

#endif
