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
		class joystick
		{
			interfaces::analog_input* x;
			interfaces::analog_input* y;
			interfaces::digital_input* button;

			public:
				joystick(unsigned char socket_number);
				~joystick();

				bool is_pressed();
				void get_xy(double& x, double& y);
				double get_x();
				double get_y();
		};
	}
}
