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
		class max_o
		{
			const socket& sock;
			interfaces::digital_output enable;
			interfaces::digital_output clr;
			devices::spi spi;

			unsigned char* data;
			unsigned int length;
			bool resized;
			unsigned int num_boards;

			public:
				max_o(unsigned char socket_number);
				~max_o();

				void set_number_of_boards(unsigned int boards);
				unsigned int get_array_size();
				void clear();
				void write_array(unsigned char* arr, unsigned int length);
				void write_pin(unsigned int board, unsigned int pin, bool value);
				unsigned char* read();
				void enable_outputs();
				void disable_outputs();
		};
	}
}
