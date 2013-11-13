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

namespace gadgeteering {
	struct socket {
		typedef unsigned long type;
		typedef unsigned char pin;

		struct pins
		{
			static const pin UNCONNECTED = 0;
			static const pin THREE = 3;
			static const pin FOUR = 4;
			static const pin FIVE = 5;
			static const pin SIX = 6;
			static const pin SEVEN = 7;
			static const pin EIGHT = 8;
			static const pin NINE = 9;
		};

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
		cpu_pin pins[10];
		spi_channel spi;
		i2c_channel i2c;
		serial_channel serial;
		analog_channel analog1;
		analog_channel analog2;
		analog_channel analog3;

		socket();
		socket(unsigned char number, type type);

		void ensure_type(type type) const;
	};
}
