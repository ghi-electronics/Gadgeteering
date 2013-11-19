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
		class button_s7
		{
			interfaces::digital_input* buttons[7];

			public:
				typedef unsigned char button;
				struct buttons
				{
					static const button ENTER = 3;
					static const button BACK = 4;
					static const button LEFT = 5;
					static const button UP = 6;
					static const button DOWN = 7;
					static const button RIGHT = 8;
					static const button FORWARD = 9;
				};

				button_s7(unsigned char socket_number);
				~button_s7();

				bool is_pressed(button b);
		};
	}
}
