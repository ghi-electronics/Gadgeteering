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

#include "Types.h"

namespace gadgeteering
{
	namespace indirectors
	{
		class digital_input;
		class digital_output;
		class digital_io;
		class analog_input;
		class analog_output;
		class pwm_output;
	}

	struct socket
	{
		typedef unsigned long type;

		struct types
		{
			static const type A = 0x00000001;
			static const type B = 0x00000002;
			static const type C = 0x00000004;
			static const type D = 0x00000008;
			static const type E = 0x00000010;
			static const type F = 0x00000020;
			static const type G = 0x00000040;
			static const type H = 0x00000080;
			static const type I = 0x00000100;
			static const type K = 0x00000200;
			static const type O = 0x00000400;
			static const type P = 0x00000800;
			static const type R = 0x00001000;
			static const type S = 0x00002000;
			static const type T = 0x00004000;
			static const type U = 0x00008000;
			static const type X = 0x00010000;
			static const type Y = 0x00020000;
			static const type Z = 0x00040000;
			static const type DL = 0x00080000;
		};

		unsigned char number;
		type type_mask;
		cpu_pin pins[11];

		spi_channel spi;
		i2c_channel i2c;
		serial_channel serial;

		analog_channel analog3;
		analog_channel analog4;
		analog_channel analog5;

		analog_out_channel analog_out;

		pwm_channel pwm7;
		pwm_channel pwm8;
		pwm_channel pwm9;

		indirectors::digital_input* digital_input_indirector;
		indirectors::digital_output* digital_output_indirector;
		indirectors::digital_io* digital_io_indirector;
		indirectors::analog_input* analog_input_indirector;
		indirectors::analog_output* analog_output_indirector;
		indirectors::pwm_output* pwm_output_indirector;

		socket();
		socket(unsigned char number, type type);
		~socket();

		void ensure_type(type type) const;
	};

	namespace indirectors
	{
		class digital_input
		{
			public:
				virtual bool read(socket_pin_number pin_number) = 0;
				virtual void set_input(socket_pin_number pin_number, resistor_mode mode) = 0;
		};

		class digital_output
		{
			public:
				virtual void write(socket_pin_number pin_number, bool value) = 0;
				virtual void set_output(socket_pin_number pin_number) = 0;
		};

		class digital_io
		{
			public:
				virtual void write(socket_pin_number pin_number, bool value) = 0;
				virtual bool read(socket_pin_number pin_number) = 0;
				virtual void set_io_mode(socket_pin_number pin_number, io_mode new_io_mode, resistor_mode new_resistor_mode) = 0;
		};

		class analog_input
		{
			public:
				virtual double read(socket_pin_number pin_number) = 0;
		};

		class analog_output
		{
			public:
				virtual void write(socket_pin_number pin_number, double voltage) = 0;
		};

		class pwm_output
		{
			public:
				virtual void set(socket_pin_number pin_number, double duty_cycle, double frequency) = 0;
		};
	}
}
