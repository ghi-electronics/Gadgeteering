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
		class relay_iso_x16
		{
			unsigned short reg_data;

			const socket& sock;
			interfaces::digital_output data;
			interfaces::digital_output clock;
			interfaces::digital_output enable;
			interfaces::digital_output latch;
			interfaces::digital_output clear;

			void write_data();

			public:
				relay_iso_x16(unsigned char socket_number);

				void disable_all_relays();
				void enable_all_relays();

				void enable_relays(unsigned short relay_mask);
				void disable_relays(unsigned short relay_mask);

				void enable_output();
				void disable_output();

				struct relays
				{
					static const unsigned short RELAY_1 = 1;
					static const unsigned short RELAY_2 = 2;
					static const unsigned short RELAY_3 = 4;
					static const unsigned short RELAY_4 = 8;
					static const unsigned short RELAY_5 = 16;
					static const unsigned short RELAY_6 = 32;
					static const unsigned short RELAY_7 = 64;
					static const unsigned short RELAY_8 = 128;
					static const unsigned short RELAY_9 = 256;
					static const unsigned short RELAY_10 = 512;
					static const unsigned short RELAY_11 = 1024;
					static const unsigned short RELAY_12 = 2048;
					static const unsigned short RELAY_13 = 4096;
					static const unsigned short RELAY_14 = 8192;
					static const unsigned short RELAY_15 = 16384;
					static const unsigned short RELAY_16 = 32768;
				};
		};
	}
}
