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
		class reflector_r3
		{
			const socket& sock;
			interfaces::analog_input left;
			interfaces::analog_input center;
			interfaces::analog_input right;
			interfaces::digital_output center_switch;

			public:
				typedef unsigned char reflector;
				struct reflectors
				{
					static const reflector LEFT = 0;
					static const reflector RIGHT = 1;
					static const reflector CENTER = 2;
				};

				reflector_r3(unsigned char socket_number);

				double read(reflector r);
		};
	}
}
