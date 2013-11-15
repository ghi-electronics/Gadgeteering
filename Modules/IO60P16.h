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
		class io60p16
		{
			unsigned char output_ports[8];
			unsigned char pin_directions[8];
			unsigned char pwms[8];
			unsigned char resistors[4][8];

			static const unsigned char INPUT_PORT_0_REGISTER = 0x00;
			static const unsigned char OUTPUT_PORT_0_REGISTER = 0x08;
			static const unsigned char PORT_SELECT_REGISTER = 0x18;

			static const unsigned char PIN_DIRECTION_REGISTER = 0x1C;
			static const unsigned char PIN_PULL_UP = 0x1D;
			static const unsigned char PIN_PULL_DOWN = 0x1E;
			static const unsigned char PIN_STRONG_DRIVE = 0x21;
			static const unsigned char PIN_HIGH_IMPEDENCE = 0x23;

			static const unsigned char ENABLE_PWM_REGISTER = 0x1A;
			static const unsigned char PWM_SELECT_REGISTER = 0x28;
			static const unsigned char PWM_CONFIG = 0x29;
			static const unsigned char PERIOD_REGISTER = 0x2A;
			static const unsigned char PULSE_WIDTH_REGISTER = 0x2B;

			static const unsigned char CLOCK_SOURCE = 0x3;

			devices::i2c* chip;

			void change_resistor(unsigned char port, unsigned char mask, unsigned char new_resistor);

		public:
			io60p16(unsigned char socket_number);
			io60p16(const socket& socket, socket_pin_number sda = 8, socket_pin_number scl = 9);
			~io60p16();

			void set_io_mode(unsigned char port, unsigned char pin, io_mode new_io_mode, resistor_mode new_resistor_mode);
			void set_pwm(unsigned char port, unsigned char pin, double duty_cycle, double frequency);
			bool read_digital(unsigned char port, unsigned char pin);
			void write_digital(unsigned char port, unsigned char pin, bool value);
		};
	}
}
