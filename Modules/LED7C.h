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
		class led_7c
		{
			interfaces::digital_output* red;
			interfaces::digital_output* green;
			interfaces::digital_output* blue;

		public:
			typedef unsigned char color;

			struct colors
			{
				static const color RED = (1 << 2) | (0 << 1) | 0;
				static const color GREEN = (0 << 2) | (1 << 1) | 0;
				static const color BLUE = (0 << 2) | (0 << 1) | 1;
				static const color YELLOW = (1 << 2) | (1 << 1) | 0;
				static const color CYAN = (0 << 2) | (1 << 1) | 1;
				static const color MAGNETA = (1 << 2) | (0 << 1) | 1;
				static const color WHITE = (1 << 2) | (1 << 1) | 1;
				static const color OFF = 0;
			};

			led_7c(unsigned char socket_number);
			~led_7c();

			void set_color(color c);
		};
	}
}
