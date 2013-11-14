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

#ifndef _BREAKOUT_H_
#define _BREAKOUT_H_

#include "../Gadgeteering.h"

namespace gadgeteering {
	namespace modules {
		using namespace gadgeteering::interfaces;

		class Breakout {
			socket* socket;

			public:
				Breakout(unsigned char socketNumber);
				~Breakout();

				interfaces::digital_input* Setupdigital_input(Socket::Pin pin, resistor_mode resistor_mode = resistor_modes::FLOATING);
				interfaces::digital_output* Setupdigital_output(Socket::Pin pin, bool initialState = false);
				interfaces::DigitalIO* SetupDigitalIO(Socket::Pin pin);
				interfaces::analog_input* Setupanalog_input(Socket::Pin pin);
				interfaces::analog_output* Setupanalog_output(Socket::Pin pin);
				interfaces::pwm_output* Setuppwm_output(Socket::Pin pin);
		};
	}
}

#endif
