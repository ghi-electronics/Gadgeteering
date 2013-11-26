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
		class compass
		{
			const socket& sock;
			devices::i2c i2c;
			interfaces::digital_input data_ready;

			unsigned char read_buffer_8[1];
			unsigned char write_buffer_8[1];
			unsigned char write_buffer_16[2];
			unsigned char read_buffer_48[6];

			static const double sq2p1;
			static const double sq2m1;
			static const double pio4;
			static const double pio2;
			static const double atan_p4;
			static const double atan_p3;
			static const double atan_p2;
			static const double atan_p1;
			static const double atan_p0;
			static const double atan_q4;
			static const double atan_q3;
			static const double atan_q2;
			static const double atan_q1;
			static const double atan_q0;

			struct registers
			{
				// From HMC5883.pdf Datasheet, pg. 12.
				//                          R/W    Bit 7    Bit 6    Bit 5    Bit 4    Bit 3    Bit 2    Bit 1    Bit 0
				// ------------------------------------------------------------------------------------------------------
				static const unsigned char CRA = 0x00;             //  R/W
				static const unsigned char CRB = 0x01;             //  R/W
				static const unsigned char MR = 0x02;             //  R/W
				static const unsigned char DXRA = 0x03;            //  R
				static const unsigned char DXRB = 0x04;            //  R
				static const unsigned char DZRA = 0x05;            //  R
				static const unsigned char DZRB = 0x06;            //  R
				static const unsigned char DYRA = 0x07;            //  R
				static const unsigned char DYRB = 0x08;            //  R
				static const unsigned char SR = 0x09;             //  R
				static const unsigned char IRA = 0x0A;             //  R
				static const unsigned char IRB = 0x0B;             //  R
				static const unsigned char IRC = 0x0C;             //  R
			};

			struct modes
			{
				static const unsigned char CONTINUOUS = 0x00;
				static const unsigned char SINGLE_MODE = 0x01;
				static const unsigned char IDLE_MODE = 0x02;
				static const unsigned char SLEEP_MODE = 0x03;
			};

			public:
				struct gains
				{
					static const unsigned char Gain1 = 0x00;
					static const unsigned char Gain2 = 0x20;
					static const unsigned char Gain3 = 0x40;
					static const unsigned char Gain4 = 0x60;
					static const unsigned char Gain5 = 0x80;
					static const unsigned char Gain6 = 0xA0;
					static const unsigned char Gain7 = 0xC0;
					static const unsigned char Gain8 = 0xE0;
				};

				compass(unsigned char socket_number);

				struct sensor_data
				{
					int x;
					int y;
					int z;
					double angle;

					sensor_data(double angle, int x, int y, int z);
				};

				sensor_data request_measurement ();
				void set_gain(unsigned char gain);

				double atan2(double y, double x);
				double atans(double x);
				double atanx(double x);
		};
	}
}
