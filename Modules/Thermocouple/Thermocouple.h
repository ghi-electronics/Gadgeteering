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

#ifndef _THERMOCOUPLE_H_
#define _THERMOCOUPLE_H_

#include <Core/Gadgeteering.h>

namespace Gadgeteering {
	namespace Modules {
		using namespace Gadgeteering::Interfaces;

		class Thermocouple : public Module {
			static const unsigned char ERROR_NOCONECT = 0x01;
			static const unsigned char ERROR_SHORTGND = 0x02;
			static const unsigned char ERROR_SHORTVCC = 0x04;

			DigitalInput* _miso;
			DigitalOutput* _clk;
			DigitalOutput* _cs;

			unsigned long ReadData();

			public:
				Thermocouple(unsigned char socketNumber);
				~Thermocouple();

				short GetExternalTemp_Celsius();
				short GetExternalTemp_Fahrenheit();
				unsigned char GetInternalTemp_Celsius();
		};
	}
}

#endif
