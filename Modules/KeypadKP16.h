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
		class keypad_kp16
		{
			interfaces::digital_output* a0;
			interfaces::digital_output* a1;
			interfaces::digital_input* r1;
			interfaces::digital_input* r2;
			interfaces::digital_input* r3;
			interfaces::digital_input* r4;

			public:
				keypad_kp16(unsigned char socket_number);
				~keypad_kp16();

				typedef unsigned char key;
				struct keys
				{
					static const key ONE = 1;
					static const key TWO = 2;
					static const key THREE = 3;
					static const key FOUR = 4;
					static const key FIVE = 5;
					static const key SIX = 6;
					static const key SEVEN = 7;
					static const key EIGHT = 8;
					static const key NINE = 9;
					static const key ZERO = 0;
					static const key A = 'A';
					static const key B = 'B';
					static const key C = 'C';
					static const key D = 'D';
					static const key STAR = '*';
					static const key POUND = '#';
				};

				bool is_key_pressed(key k);
		};
	}
}
