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
		class touch_l12
		{ 
			static const unsigned char I2C_ADDRESS = 0x2B;
			static const unsigned char IRQ_SRC = 0x0;
			static const unsigned char CAP_STAT_MSB = 0x1;
			static const unsigned char CAP_STAT_LSB = 0x2;
			static const unsigned char WHL_POS_MSB = 0x3;
			static const unsigned char WHL_POS_LSB = 0x4;
			static const unsigned char WHEELS = 12;

			const socket& sock;
			interfaces::digital_output reset_port;
			devices::i2c device;

			void reset();
			void configure_spm();
			void write_spm(unsigned char address, const unsigned char data[9]);

			public:
				typedef unsigned char direction;
				struct directions
				{
					static const direction LEFT = 0;
					static const direction RIGHT = 1;
				};

				touch_l12(unsigned char socket_number);

				bool is_slider_pressed();
				double get_slider_position();
				direction get_slider_direction();
		};
	}
}
