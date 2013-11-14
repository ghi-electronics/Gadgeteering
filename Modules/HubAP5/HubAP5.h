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

#include <Core/Gadgeteering.h>

#include "../IO60P16/IO60P16.h"
#include "../ADS7830/ADS7830.h"

namespace gadgeteering
{
	namespace modules
	{
		class hub_ap5
		{
			unsigned char socket_start;

			io60p16* io60_chip;
			ads_7830* analog_chip;

			void create_sockets();

			class indirected_digital_input : public indirectors::digital_input
			{
				const socket& sock;
				hub_ap5& hub;

				public:
					indirected_digital_input(const socket& socket, hub_ap5& hub);

					virtual bool read(socket::pin pin_number);
					virtual void set_input(socket::pin pin_number, resistor_mode mode);
			};

			class indirected_digital_output : public indirectors::digital_output
			{
				const socket& sock;
				hub_ap5& hub;

				public:
					indirected_digital_output(const socket& socket, hub_ap5& hub);

					virtual void write(socket::pin pin_number, bool value);
					virtual void set_output(socket::pin pin_number);
			};

			class indirected_digital_io : public indirectors::digital_io
			{
				const socket& sock;
				hub_ap5& hub;

				public:
					indirected_digital_io(const socket& socket, hub_ap5& hub);

					virtual void write(socket::pin pin_number, bool value);
					virtual bool read(socket::pin pin_number);
					virtual void set_io_mode(socket::pin pin_number, io_mode new_io_mode, resistor_mode new_resistor_mode);
			};

			class indirected_analog_input : public indirectors::analog_input
			{
				const socket& sock;
				hub_ap5& hub;

				public:
					indirected_analog_input(const socket& socket, hub_ap5& hub);

					virtual double read(socket::pin pin_number);
			};

			class indirected_pwm_output : public indirectors::pwm_output
			{
				const socket& sock;
				hub_ap5& hub;

				public:
					indirected_pwm_output(const socket& socket, hub_ap5& hub);

					virtual void set(socket::pin pin_number, double duty_cycle, double frequency);
			};

			public:
				struct pins
				{
					static const cpu_pin P0_0 = 16 * 0 + 0;
					static const cpu_pin P0_1 = 16 * 0 + 1;
					static const cpu_pin P0_2 = 16 * 0 + 2;
					static const cpu_pin P0_3 = 16 * 0 + 3;
					static const cpu_pin P0_4 = 16 * 0 + 4;
					static const cpu_pin P0_5 = 16 * 0 + 5;
					static const cpu_pin P0_6 = 16 * 0 + 6;
					static const cpu_pin P0_7 = 16 * 0 + 7;

					static const cpu_pin P1_0 = 16 * 1 + 0;
					static const cpu_pin P1_1 = 16 * 1 + 1;
					static const cpu_pin P1_2 = 16 * 1 + 2;
					static const cpu_pin P1_3 = 16 * 1 + 3;
					static const cpu_pin P1_4 = 16 * 1 + 4;
					static const cpu_pin P1_5 = 16 * 1 + 5;
					static const cpu_pin P1_6 = 16 * 1 + 6;
					static const cpu_pin P1_7 = 16 * 1 + 7;

					static const cpu_pin P2_0 = 16 * 2 + 0;
					static const cpu_pin P2_1 = 16 * 2 + 1;
					static const cpu_pin P2_2 = 16 * 2 + 2;
					static const cpu_pin P2_3 = 16 * 2 + 3;
					static const cpu_pin P2_4 = 16 * 2 + 4;
					static const cpu_pin P2_5 = 16 * 2 + 5;
					static const cpu_pin P2_6 = 16 * 2 + 6;
					static const cpu_pin P2_7 = 16 * 2 + 7;

					static const cpu_pin P3_0 = 16 * 3 + 0;
					static const cpu_pin P3_1 = 16 * 3 + 1;
					static const cpu_pin P3_2 = 16 * 3 + 2;
					static const cpu_pin P3_3 = 16 * 3 + 3;
					static const cpu_pin P3_4 = 16 * 3 + 4;
					static const cpu_pin P3_5 = 16 * 3 + 5;
					static const cpu_pin P3_6 = 16 * 3 + 6;
					static const cpu_pin P3_7 = 16 * 3 + 7;

					static const cpu_pin P4_0 = 16 * 4 + 0;
					static const cpu_pin P4_1 = 16 * 4 + 1;
					static const cpu_pin P4_2 = 16 * 4 + 2;
					static const cpu_pin P4_3 = 16 * 4 + 3;
					static const cpu_pin P4_4 = 16 * 4 + 4;
					static const cpu_pin P4_5 = 16 * 4 + 5;
					static const cpu_pin P4_6 = 16 * 4 + 6;
					static const cpu_pin P4_7 = 16 * 4 + 7;

					static const cpu_pin P5_0 = 16 * 5 + 0;
					static const cpu_pin P5_1 = 16 * 5 + 1;
					static const cpu_pin P5_2 = 16 * 5 + 2;
					static const cpu_pin P5_3 = 16 * 5 + 3;
					static const cpu_pin P5_4 = 16 * 5 + 4;
					static const cpu_pin P5_5 = 16 * 5 + 5;
					static const cpu_pin P5_6 = 16 * 5 + 6;
					static const cpu_pin P5_7 = 16 * 5 + 7;

					static const cpu_pin P6_0 = 16 * 6 + 0;
					static const cpu_pin P6_1 = 16 * 6 + 1;
					static const cpu_pin P6_2 = 16 * 6 + 2;
					static const cpu_pin P6_3 = 16 * 6 + 3;
					static const cpu_pin P6_4 = 16 * 6 + 4;
					static const cpu_pin P6_5 = 16 * 6 + 5;
					static const cpu_pin P6_6 = 16 * 6 + 6;
					static const cpu_pin P6_7 = 16 * 6 + 7;

					static const cpu_pin P7_0 = 16 * 7 + 0;
					static const cpu_pin P7_1 = 16 * 7 + 1;
					static const cpu_pin P7_2 = 16 * 7 + 2;
					static const cpu_pin P7_3 = 16 * 7 + 3;
					static const cpu_pin P7_4 = 16 * 7 + 4;
					static const cpu_pin P7_5 = 16 * 7 + 5;
					static const cpu_pin P7_6 = 16 * 7 + 6;
					static const cpu_pin P7_7 = 16 * 7 + 7;
				};

				hub_ap5(unsigned char socket_number);
				~hub_ap5();

				const unsigned char socket_1;
				const unsigned char socket_2;
				const unsigned char socket_3;
				const unsigned char socket_4;
				const unsigned char socket_5;
				const unsigned char socket_6;
				const unsigned char socket_7;
				const unsigned char socket_8;

				void set_io_mode(cpu_pin pin, io_mode new_io_mode, resistor_mode new_resistor_mode);
				void set_pwm(pwm_channel channel, double duty_cycle, double frequency);
				bool read_digital(cpu_pin pin);
				void write_digital(cpu_pin pin, bool value);
				double read_analog(analog_channel channel);
		};
	}
}
