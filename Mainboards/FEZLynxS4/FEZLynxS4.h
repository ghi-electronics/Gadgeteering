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

#include <map>

#include <Core/Gadgeteering.h>
#include <Modules/IO60P16/IO60P16.h>
#include <Modules/ADS7830/ADS7830.h>

#include "FTDIDriver.h"

namespace gadgeteering
{
    namespace mainboards
    {
        class fez_lynx_s4: public base_mainboard
        {
			std::map<unsigned char, socket> sockets;
			std::map<analog_channel, std::pair<cpu_pin, unsigned char>> analog_channel_to_pin_map;
			std::map<analog_channel, cpu_pin> pwm_channel_to_pin_map;
            ftdi_channel channels[4];
           
			bool extender_present;
			modules::io60p16* extender;
			modules::ads_7830* extender_analog_converter;
			modules::ads_7830* analog_converter;

			void create_sockets();

            public:
                struct pins
                {
					static const cpu_pin P0_0 = 16 * 0 + 0 + 128;
					static const cpu_pin P0_1 = 16 * 0 + 1 + 128;
					static const cpu_pin P0_2 = 16 * 0 + 2 + 128;
					static const cpu_pin P0_3 = 16 * 0 + 3 + 128;
					static const cpu_pin P0_4 = 16 * 0 + 4 + 128;
					static const cpu_pin P0_5 = 16 * 0 + 5 + 128;
					static const cpu_pin P0_6 = 16 * 0 + 6 + 128;
					static const cpu_pin P0_7 = 16 * 0 + 7 + 128;

					static const cpu_pin P1_0 = 16 * 1 + 0 + 128;
					static const cpu_pin P1_1 = 16 * 1 + 1 + 128;
					static const cpu_pin P1_2 = 16 * 1 + 2 + 128;
					static const cpu_pin P1_3 = 16 * 1 + 3 + 128;
					static const cpu_pin P1_4 = 16 * 1 + 4 + 128;
					static const cpu_pin P1_5 = 16 * 1 + 5 + 128;
					static const cpu_pin P1_6 = 16 * 1 + 6 + 128;
					static const cpu_pin P1_7 = 16 * 1 + 7 + 128;

					static const cpu_pin P2_0 = 16 * 2 + 0 + 128;
					static const cpu_pin P2_1 = 16 * 2 + 1 + 128;
					static const cpu_pin P2_2 = 16 * 2 + 2 + 128;
					static const cpu_pin P2_3 = 16 * 2 + 3 + 128;
					static const cpu_pin P2_4 = 16 * 2 + 4 + 128;
					static const cpu_pin P2_5 = 16 * 2 + 5 + 128;
					static const cpu_pin P2_6 = 16 * 2 + 6 + 128;
					static const cpu_pin P2_7 = 16 * 2 + 7 + 128;

					static const cpu_pin P3_0 = 16 * 3 + 0 + 128;
					static const cpu_pin P3_1 = 16 * 3 + 1 + 128;
					static const cpu_pin P3_2 = 16 * 3 + 2 + 128;
					static const cpu_pin P3_3 = 16 * 3 + 3 + 128;
					static const cpu_pin P3_4 = 16 * 3 + 4 + 128;
					static const cpu_pin P3_5 = 16 * 3 + 5 + 128;
					static const cpu_pin P3_6 = 16 * 3 + 6 + 128;
					static const cpu_pin P3_7 = 16 * 3 + 7 + 128;

					static const cpu_pin P4_0 = 16 * 4 + 0 + 128;
					static const cpu_pin P4_1 = 16 * 4 + 1 + 128;
					static const cpu_pin P4_2 = 16 * 4 + 2 + 128;
					static const cpu_pin P4_3 = 16 * 4 + 3 + 128;
					static const cpu_pin P4_4 = 16 * 4 + 4 + 128;
					static const cpu_pin P4_5 = 16 * 4 + 5 + 128;
					static const cpu_pin P4_6 = 16 * 4 + 6 + 128;
					static const cpu_pin P4_7 = 16 * 4 + 7 + 128;

					static const cpu_pin P5_0 = 16 * 5 + 0 + 128;
					static const cpu_pin P5_1 = 16 * 5 + 1 + 128;
					static const cpu_pin P5_2 = 16 * 5 + 2 + 128;
					static const cpu_pin P5_3 = 16 * 5 + 3 + 128;
					static const cpu_pin P5_4 = 16 * 5 + 4 + 128;
					static const cpu_pin P5_5 = 16 * 5 + 5 + 128;
					static const cpu_pin P5_6 = 16 * 5 + 6 + 128;
					static const cpu_pin P5_7 = 16 * 5 + 7 + 128;

					static const cpu_pin P6_0 = 16 * 6 + 0 + 128;
					static const cpu_pin P6_1 = 16 * 6 + 1 + 128;
					static const cpu_pin P6_2 = 16 * 6 + 2 + 128;
					static const cpu_pin P6_3 = 16 * 6 + 3 + 128;
					static const cpu_pin P6_4 = 16 * 6 + 4 + 128;
					static const cpu_pin P6_5 = 16 * 6 + 5 + 128;
					static const cpu_pin P6_6 = 16 * 6 + 6 + 128;
					static const cpu_pin P6_7 = 16 * 6 + 7 + 128;

					static const cpu_pin P7_0 = 16 * 7 + 0 + 128;
					static const cpu_pin P7_1 = 16 * 7 + 1 + 128;
					static const cpu_pin P7_2 = 16 * 7 + 2 + 128;
					static const cpu_pin P7_3 = 16 * 7 + 3 + 128;
					static const cpu_pin P7_4 = 16 * 7 + 4 + 128;
					static const cpu_pin P7_5 = 16 * 7 + 5 + 128;
					static const cpu_pin P7_6 = 16 * 7 + 6 + 128;
					static const cpu_pin P7_7 = 16 * 7 + 7 + 128;

					static const cpu_pin AD0 = 16 * 0 + 0;
					static const cpu_pin AD1 = 16 * 0 + 1;
					static const cpu_pin AD2 = 16 * 0 + 2;
					static const cpu_pin AD3 = 16 * 0 + 3;
					static const cpu_pin AD4 = 16 * 0 + 4;
					static const cpu_pin AD5 = 16 * 0 + 5;
					static const cpu_pin AD6 = 16 * 0 + 6;
					static const cpu_pin AD7 = 16 * 0 + 7;

					static const cpu_pin BD0 = 16 * 1 + 0;
					static const cpu_pin BD1 = 16 * 1 + 1;
					static const cpu_pin BD2 = 16 * 1 + 2;
					static const cpu_pin BD3 = 16 * 1 + 3;
					static const cpu_pin BD4 = 16 * 1 + 4;
					static const cpu_pin BD5 = 16 * 1 + 5;
					static const cpu_pin BD6 = 16 * 1 + 6;
					static const cpu_pin BD7 = 16 * 1 + 7;

					static const cpu_pin CD0 = 16 * 2 + 0;
					static const cpu_pin CD1 = 16 * 2 + 1;
					static const cpu_pin CD2 = 16 * 2 + 2;
					static const cpu_pin CD3 = 16 * 2 + 3;
					static const cpu_pin CD4 = 16 * 2 + 4;
					static const cpu_pin CD5 = 16 * 2 + 5;
					static const cpu_pin CD6 = 16 * 2 + 6;
					static const cpu_pin CD7 = 16 * 2 + 7;

					static const cpu_pin DD0 = 16 * 3 + 0;
					static const cpu_pin DD1 = 16 * 3 + 1;
					static const cpu_pin DD2 = 16 * 3 + 2;
					static const cpu_pin DD3 = 16 * 3 + 3;
					static const cpu_pin DD4 = 16 * 3 + 4;
					static const cpu_pin DD5 = 16 * 3 + 5;
					static const cpu_pin DD6 = 16 * 3 + 6;
					static const cpu_pin DD7 = 16 * 3 + 7;
				};
				
				fez_lynx_s4(bool extender_present = true);
				virtual ~fez_lynx_s4();

				virtual socket& register_socket(socket s);
				virtual const socket& get_socket(unsigned char number);

				virtual void set_debug_led(bool state);

				virtual void set_io_mode(cpu_pin pin, io_mode new_io_mode, resistor_mode new_resistor_mode);
				virtual void write_digital(cpu_pin pin, bool value);
				virtual bool read_digital(cpu_pin pin);
				virtual void write_analog(analog_out_channel channel, double voltage);
				virtual double read_analog(analog_channel channel);
				virtual void set_pwm(pwm_channel channel, double duty_cycle, double frequency);

				virtual void spi_read_write(spi_channel channel, const unsigned char* write_buffer, unsigned char* read_buffer, unsigned int count, spi_configuration& config, bool deselect_after);
				virtual bool i2c_write(i2c_channel channel, const unsigned char* buffer, unsigned int length, bool send_start, bool send_stop);
				virtual bool i2c_read(i2c_channel channel, unsigned char* buffer, unsigned int length, bool send_start, bool send_stop);
				virtual unsigned int serial_write(serial_channel  channel, const unsigned char* buffer, unsigned int count, serial_configuration& config);
				virtual unsigned int serial_read(serial_channel  channel, unsigned char* buffer, unsigned int count, serial_configuration& config);
				virtual unsigned int serial_available(serial_channel channel);
        };
    }
}
