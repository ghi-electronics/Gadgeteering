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

#pragma once

#include "../Gadgeteering.h"

namespace gadgeteering
{
	namespace modules
	{
		class display_n18
		{
#if (defined ARDUINO_AVR_UNO || defined ARDUINO_AVR_MEDUSA)
			static const unsigned char STEP = 1;
#else
			static const unsigned char STEP = 12;
#endif

			const socket& sock;
			interfaces::digital_output reset_pin;
			interfaces::digital_output backlight_pin;
			interfaces::digital_output rs_pin;
			devices::spi spi;

			void write_command(unsigned char command);
			void write_data(unsigned char data);
			void write_data(const unsigned char* data, unsigned int length);

			void reset();
			void initialize();
			void set_clipping_area(unsigned char x, unsigned char y, unsigned char width, unsigned char height);

			public:
				display_n18(unsigned char socket_number);

				static const unsigned int WIDTH = 128;
				static const unsigned int HEIGHT = 160;
				static const unsigned char CHAR_WIDTH = 5;
				static const unsigned char CHAR_HEIGHT = 8;
				static const unsigned char CHAR_SPACING = 1;

				static unsigned short rgb_to_short(unsigned char r, unsigned char g, unsigned char b);

				void clear(unsigned short back_color = 0x0000);
				void draw(const unsigned char* data, int x, int y, int width, int height);
				void draw(const unsigned short* data, int x, int y, int width, int height);
				void set_pixel(int x, int y, unsigned short fore_color);

				void fill_rect(int x, int y, int width, int height, unsigned short fore_color);
				void draw_rect(int x, int y, int width, int height, unsigned short fore_color);

				void fill_circle(int x, int y, int radius, unsigned short fore_color);
				void draw_circle(int x, int y, int radius, unsigned short fore_color);

				void draw_line(int x0, int y0, int x1, int y1, unsigned short fore_color);

				void draw_character(int x, int y, const char character, unsigned short fore_color, unsigned short back_color, unsigned char font_size = 1);
				void draw_string(int x, int y, const char* str, unsigned short fore_color, unsigned short back_color, unsigned char font_size = 1);

				void setup_draw(int x, int y, int width, int height);
				void send_draw_data(const unsigned char* data, unsigned int length, bool is_end = false);
				void send_draw_data(const unsigned short* data, unsigned int length, bool is_end = false);
		};
	}
}
