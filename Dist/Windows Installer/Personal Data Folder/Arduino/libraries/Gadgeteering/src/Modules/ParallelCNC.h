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
		class parallel_cnc : public daisy_link::module
		{
			static const unsigned char GHI_DAISYLINK_MANUFACTURER = 0x10;
			static const unsigned char GHI_DAISYLINK_TYPE_PARALLELCNC = 0x03;
			static const unsigned char GHI_DAISYLINK_VERSION_PARALLELCNC = 0x01;

			struct registers
			{
				static const unsigned char ACTIVATE = 0;
				static const unsigned char STATUS = 1;
				static const unsigned char XSTEP1 = 2;
				static const unsigned char XSTEP2 = 3;
				static const unsigned char XSTEP3 = 4;
				static const unsigned char XSTEP4 = 5;
				static const unsigned char YSTEP1 = 6;
				static const unsigned char YSTEP2 = 7;
				static const unsigned char YSTEP3 = 8;
				static const unsigned char YSTEP4 = 9;
				static const unsigned char ZSTEP1 = 10;
				static const unsigned char ZSTEP2 = 11;
				static const unsigned char ZSTEP3 = 12;
				static const unsigned char ZSTEP4 = 13;
				static const unsigned char DIRECTION = 14;
				static const unsigned char ENABLE = 15;
				static const unsigned char VALUE = 16;
				static const unsigned char LED = 17;
				static const unsigned char X_ENABLE = 18;
				static const unsigned char X_DIR = 19;
				static const unsigned char X_STEP = 20;
				static const unsigned char Y_ENABLE = 21;
				static const unsigned char Y_DIR = 22;
				static const unsigned char Y_STEP = 23;
				static const unsigned char Z_ENABLE = 24;
				static const unsigned char Z_DIR = 25;
				static const unsigned char Z_STEP = 26;
				static const unsigned char A_ENABLE = 27;
				static const unsigned char A_DIR = 28;
				static const unsigned char A_STEP = 29;
				static const unsigned char B_ENABLE = 30;
				static const unsigned char B_DIR = 31;
				static const unsigned char B_STEP = 32;
			};

			void write_register(unsigned char address, unsigned char value);
			unsigned char read_register(unsigned char address);

			public:
				typedef unsigned char axis;
				struct axes
				{
					static const axis X = 0;
					static const axis Y = 1;
					static const axis Z = 2;
					static const axis A = 3;
					static const axis B = 4;
				};

				typedef unsigned char axis_mask;
				struct axis_masks
				{
					static const axis_mask X = 32;
					static const axis_mask Y = 16;
					static const axis_mask Z = 8;
					static const axis_mask A = 4;
					static const axis_mask B = 2;
				};

				parallel_cnc(unsigned char socket_number);

				void configure_pins(unsigned char x_enable, unsigned char x_dir, unsigned char x_step, unsigned char y_enable, unsigned char y_dir, unsigned char y_step, unsigned char z_enable, unsigned char z_dir, unsigned char z_step, unsigned char a_enable, unsigned char a_dir, unsigned char a_step, unsigned char b_enable, unsigned char b_dir, unsigned char b_step);
				void move_motor(axis a, int steps, bool positive_direction);
				void set_all_motors(int x_steps, int y_steps, int z_steps, int a_steps, int b_steps);
				void enable_motor(axis_mask a);
				void disable_motor(axis_mask a);
				void terminate_movements();
				void emergency_stop(bool lock_motors_in_place);
		};
	}
}
