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

#ifndef _EXTENDER_H_
#define _EXTENDER_H_

#include <core/Gadgeteering.h>

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class Extender : public Module {
			Socket* socket;

			public:
				Extender(unsigned char socketNumber);
				~Extender();

				Interfaces::DigitalInput* SetupDigitalInput(Socket::Pin pin, ResistorMode resistorMode = ResistorModes::FLOATING);
				Interfaces::DigitalOutput* SetupDigitalOutput(Socket::Pin pin, bool initialState = false);
				Interfaces::DigitalIO* SetupDigitalIO(Socket::Pin pin);
				Interfaces::AnalogInput* SetupAnalogInput(Socket::Pin pin);
				Interfaces::AnalogOutput* SetupAnalogOutput(Socket::Pin pin);
				Interfaces::PWMOutput* SetupPWMOutput(Socket::Pin pin);
		};
	}
}

#endif
