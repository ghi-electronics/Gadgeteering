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
		class radio_fm1
		{
			const socket& sock; 
			devices::i2c i2c;
			interfaces::digital_output reset_pin;
			interfaces::digital_input sen_pin;
			unsigned short registers[16];
			unsigned short volume;

			struct registers
			{
				static const unsigned char DEVICEID = 0x00;
				static const unsigned char CHIPID = 0x01;
				static const unsigned char POWERCFG = 0x02;
				static const unsigned char CHANNEL = 0x03;
				static const unsigned char SYSCONFIG1 = 0x04;
				static const unsigned char SYSCONFIG2 = 0x05;
				static const unsigned char STATUSRSSI = 0x0A;
				static const unsigned char READCHAN = 0x0B;
				static const unsigned char RDSA = 0x0C;
				static const unsigned char RDSB = 0x0D;
				static const unsigned char RDSC = 0x0E;
				static const unsigned char RDSD = 0x0F;
			};

			static const unsigned char I2C_ADDRESS = 0x10;

			//Register 0x02 - POWERCFG
			static const unsigned char BIT_SMUTE = 15;
			static const unsigned char BIT_DMUTE = 14;
			static const unsigned char BIT_SKMODE = 10;
			static const unsigned char BIT_SEEKUP = 9;
			static const unsigned char BIT_SEEK = 8;

			//Register 0x03 - CHANNEL
			static const unsigned char BIT_TUNE = 15;

			//Register 0x04 - SYSCONFIG1
			static const unsigned char BIT_RDS = 12;
			static const unsigned char BIT_DE = 11;

			//Register 0x05 - SYSCONFIG2
			static const unsigned char BIT_SPACE1 = 5;
			static const unsigned char BIT_SPACE0 = 4;

			//Register 0x0A - STATUSRSSI
			static const unsigned char BIT_RDSR = 15;
			static const unsigned char BIT_STC = 14;
			static const unsigned char BIT_SFBL = 13;
			static const unsigned char BIT_AFCRL = 12;
			static const unsigned char BIT_RDSS = 11;
			static const unsigned char BIT_STEREO = 8;

			void read_registers();
			void update_registers();

			public:
				typedef unsigned char seek_direction;
				struct seek_directions
				{
					static const seek_direction FORWARD = 0;
					static const seek_direction BACKWARD = 1;
				};

				static const double MAX_CHANNEL;
				static const double MIN_CHANNEL;
				static const double INVALID_CHANNEL;
				static const int MIN_VOLUME = 0;
				static const int MAX_VOLUME = 15;

				radio_fm1(unsigned char socket_number);

				void increase_volume();
				void decrease_volume();
				void set_volume(unsigned short new_volume);
				unsigned short get_volume();
				void set_channel(double new_channelS);
				double get_channel();
				double seek(seek_direction direction);
		};
	}
}
