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
		class multicolor_led : public daisy_link::module
		{
			struct registers
			{
				static const unsigned char R = 0;
				static const unsigned char G = 1;
				static const unsigned char B = 2;
				static const unsigned char Configuration = 3;
				static const unsigned char ResetTimers = 4;
				static const unsigned char Color1 = 5;
			};

			typedef unsigned char mode;
			struct modes
			{
				static const mode Off = 0;
				static const mode Constant = 1;
				static const mode blink_once = 2;
				static const mode blink_repeatedly = 3;
				static const mode fade_once = 4;
				static const mode fade_repeatedly = 5;
				static const mode blink_onceInt = 6;
				static const mode blink_repeatedlyInt = 7;
				static const mode fade_onceInt = 8;
				static const mode fade_repeatedlyInt = 9;
			};

			static const unsigned char GHI_DAISYLINK_MANUFACTURER = 0x10;
			static const unsigned char GHI_DAISYLINK_TYPE_MULTICOLORLED = 0x01;
			static const unsigned char GHI_DAISYLINK_VERSION_MULTICOLORLED = 0x01;

			bool green_blue_swapped;

			void send_command(color color_1, unsigned int blink_time_1, color color_2, unsigned int blink_time_2, mode mode);
			void send_command(color c, mode mode);
			void send_command(color c);

			public:
				multicolor_led(unsigned char socket_number);

				bool swap_green_blue();

				void turn_blue();
				void turn_red();
				void turn_green();
				void turn_off();
				void turn_white();
				void turn_color(color c);
				void set_red_intensity(unsigned char intensity);
				void set_red_intensity(int intensity);
				void set_green_intensity(unsigned char intensity);
				void set_green_intensity(int intensity);
				void set_blue_intensity(unsigned char intensity);
				void set_blue_intensity(int intensity);
				void add_red();
				void remove_red();
				void add_green();
				void remove_green();
				void add_blue();
				void remove_blue();
				color get_current_color();
				void blink_once(color c);
				void blink_once(color c, unsigned int blink_time);
				void blink_once(color blink_color, unsigned int blink_time, color end_color);
				void blink_repeatedly(color c);
				void blink_repeatedly(color color_1, unsigned int blink_time_1, color color_2, unsigned int blink_time_2);
				void fade_once(color c);
				void fade_once(color c, unsigned int fadeTime);
				void fade_once(color from_color, unsigned int fadeTime, color to_color);
				void fade_repeatedly(color c);
				void fade_repeatedly(color color_1, unsigned int fade_time_1, color color_2, unsigned int fade_time_2);
		};
	}
}
