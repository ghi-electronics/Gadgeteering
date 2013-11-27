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
		class button
		{
			const socket& sock;
			interfaces::digital_input input;
			interfaces::digital_output led;
			bool led_state;

			public:
				button(unsigned char socket_number);

				bool is_pressed();
				void turn_led_on();
				void turn_led_off();
				void toggle_led();
				void set_led(bool state);
				bool is_led_on();
		};
	}
}
