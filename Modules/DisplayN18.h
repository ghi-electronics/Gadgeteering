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

#ifndef _DISPLAYN18_H_
#define _DISPLAYN18_H_

#include <Core/Gadgeteering.h>

namespace gadgeteering {
	namespace modules {
		using namespace gadgeteering::interfaces;

		class DisplayN18 {
#ifndef LYNX
			static const unsigned char STEP_X = 4;
			static const unsigned char STEP_Y = 5;
			static const unsigned char STEP = 1;
#else
			static const unsigned char STEP_X = 128;
			static const unsigned char STEP_Y = 160;
			static const unsigned char STEP = 160;
#endif

			digital_output* resetPin;
			digital_output* backlightPin;
			digital_output* rsPin;
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

				static const unsigned int WIDTH = 128;
				static const unsigned int HEIGHT = 160;
				static const unsigned char CHAR_WIDTH = 5;
				static const unsigned char CHAR_HEIGHT = 8;
				static const unsigned char CHAR_SPACING = 1;

				static unsigned short rgbToShort(unsigned char r, unsigned char g, unsigned char b);

				void clear(unsigned short color = 0x0000);
				void draw(const unsigned char* data, int x, int y, int width, int height);
				void draw(const unsigned short* data, int x, int y, int width, int height);
				void setPixel(int x, int y, unsigned short color);

				void fillRect(int x, int y, int width, int height, unsigned short color);
				void drawRect(int x, int y, int width, int height, unsigned short color);

				void fillCircle(int x, int y, int radius, unsigned short color);
				void drawCircle(int x, int y, int radius, unsigned short color);

				void drawLine(int x0, int y0, int x1, int y1, unsigned short color);

				void drawCharacter(int x, int y, const char character, unsigned short foreColor, unsigned short backColor, unsigned char fontSize = 1);
				void drawString(int x, int y, const char* str, unsigned short foreColor, unsigned short backColor, unsigned char fontSize = 1);

				void setupDraw(int x, int y, int width, int height);
				void sendDrawData(const unsigned char* data, unsigned int length, bool deselectChip = false);
				void sendDrawData(const unsigned short* data, unsigned int length, bool deselectChip = false);


		};
	}
}

#endif
