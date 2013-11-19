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
		class character_display
		{
			const socket& sock;

			interfaces::digital_output lcd_rs;
			interfaces::digital_output lcd_e;
			
			interfaces::digital_output lcd_d4;
			interfaces::digital_output lcd_d5;
			interfaces::digital_output lcd_d6;
			interfaces::digital_output lcd_d7;
			
			interfaces::digital_output backlight;

			static const unsigned char DISP_ON = 0xC;
			static const unsigned char CLR_DISP = 1;
			static const unsigned char CUR_HOME = 2;
			static const unsigned char SET_CURSOR = 0x80;

			void send_nibble(unsigned char b);
			void send_byte(unsigned char b);
			void send_command(unsigned char c);

			public:
				character_display(unsigned char socket_number);

				void print(const char* string);
				void print(char character);
				void clear();
				void cursor_home();
				void set_cursor(unsigned char row, unsigned char col);
				void set_backlight(bool state);
		};
	}
}
