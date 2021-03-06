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
		class tunes
		{
			const socket& sock;
			interfaces::pwm_output pwm;

			public:
				tunes(unsigned char socket_number);

				void set(double frequency, double duty_cycle);
				void set_frequency(double frequency);
				void set_duty_cycle(double duty_cycle);
		};
	}
}
