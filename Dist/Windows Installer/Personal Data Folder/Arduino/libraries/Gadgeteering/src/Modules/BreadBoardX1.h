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
		class bread_board_x1
		{
			const socket& sock;

			public:
				bread_board_x1(unsigned char socket_number);

				interfaces::digital_input setup_digital_input(unsigned char pin_number, resistor_mode initial_resistor_mode = resistor_modes::FLOATING);
				interfaces::digital_output setup_digital_output(unsigned char pin_number, bool initial_state = false);
				interfaces::digital_io setup_digital_io(unsigned char pin_number);
				interfaces::analog_input setup_analog_input(unsigned char pin_number);
				interfaces::analog_output setup_analog_output(unsigned char pin_number);
				interfaces::pwm_output setup_pwm_output(unsigned char pin_number);
		};
	}
}
