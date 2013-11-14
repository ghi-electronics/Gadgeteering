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

#include <Core/Gadgeteering.h>

namespace gadgeteering
{
	namespace modules
	{
		class led_strip
		{
			static const unsigned char LEDS = 7;

			interfaces::digital_output* ports[led_strip::LEDS];

		public:
			led_strip(unsigned char socket_number);
			~led_strip();

			void turn_on_led(unsigned char led, bool only_led = false);
			void turn_off_led(unsigned char led);

			void turn_all_on();
			void turn_all_off();

			void set(unsigned char led, bool state);

			void animate(unsigned int switch_time, bool clockwise, bool turn_on, bool remain_on = true);
		};
	}
}
