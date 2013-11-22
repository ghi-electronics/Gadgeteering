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
		class daq_8b
		{
			const socket& sock;
			interfaces::digital_input miso;
			interfaces::digital_output mosi;
			interfaces::digital_output clock;
			interfaces::digital_output cs;

			unsigned short spi_write_read(unsigned short data);

			public:
				typedef unsigned short channel;
				struct channels
				{
					static const channel P1 = 0x0200;
					static const channel P2 = 0x0400;
					static const channel P3 = 0x0600;
					static const channel P4 = 0x0800;
					static const channel P5 = 0x0A00;
					static const channel P6 = 0x0C00;
					static const channel P7 = 0x0E00;
					static const channel P8 = 0x0000;
				};

				daq_8b(unsigned char socket_number);

				double get_reading(channel c);
		};
	}
}
