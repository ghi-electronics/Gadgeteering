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
		class thermocouple
		{
			static const unsigned char ERROR_NOCONECT = 0x01;
			static const unsigned char ERROR_SHORTGND = 0x02;
			static const unsigned char ERROR_SHORTVCC = 0x04;

			const socket& sock;
			interfaces::digital_input miso;
			interfaces::digital_output clk;
			interfaces::digital_output cs;

			unsigned long read_data();

			public:
				thermocouple(unsigned char socket_number);

				int get_external_temp_celsius();
				int get_external_temp_fahrenheit();
				int get_internal_temp_celsius();
				int get_internal_temp_fahrenheit();
		};
	}
}
