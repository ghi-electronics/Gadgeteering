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
				void drawRaw(const unsigned char* data, int x, int y, int width, int height);
				void setPixel(int x, int y, unsigned short color);
				
				void fillRect(int x, int y, int width, int height, unsigned short color);
				void drawRect(int x, int y, int width, int height, unsigned short color);
				
				void fillCircle(int x, int y, int radius, unsigned short color);
				void drawCircle(int x, int y, int radius, unsigned short color);
				
				void drawLine(int x0, int y0, int x1, int y1, unsigned short color);
		};
	}
}

#endif
