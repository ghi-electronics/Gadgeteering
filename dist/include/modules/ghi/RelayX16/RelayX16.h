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

#ifndef _RELAYX16_H_
#define _RELAYX16_H_

#include "../../../core/Interfaces.hpp"
#include "../../../core/Mainboard.hpp"
#include "../../../core/System.hpp"
#include "../../../core/Types.hpp"
#include "../../../core/Module.hpp"

namespace GHI
{
	namespace Modules
	{
		class RelayX16 : protected Module
		{
			protected:
				unsigned short regData;

				Interfaces::DigitalOutput *data;
				Interfaces::DigitalOutput *clock;
				Interfaces::DigitalOutput *latch;
				Interfaces::DigitalOutput *enable;
				Interfaces::DigitalOutput *clear;

				void writeRegisterData();

			public:
				RelayX16(int socket);

				void disableAllRelays();
				void enableAllRelays();

				void enableRelays(unsigned short relays);
				void disableRelays(unsigned short relays);

				void enableOutput();
				void disableOutput();

				class Relays
				{
					Relays();
					public:
						static const unsigned short Relay_1 = 1;
						static const unsigned short Relay_2 = 2;
						static const unsigned short Relay_3 = 4;
						static const unsigned short Relay_4 = 8;
						static const unsigned short Relay_5 = 16;
						static const unsigned short Relay_6 = 32;
						static const unsigned short Relay_7 = 64;
						static const unsigned short Relay_8 = 128;
						static const unsigned short Relay_9 = 256;
						static const unsigned short Relay_10 = 512;
						static const unsigned short Relay_11 = 1024;
						static const unsigned short Relay_12 = 2048;
						static const unsigned short Relay_13 = 4096;
						static const unsigned short Relay_14 = 8192;
						static const unsigned short Relay_15 = 16384;
						static const unsigned short Relay_16 = 32768;
				};
		};
	}
}

#endif
