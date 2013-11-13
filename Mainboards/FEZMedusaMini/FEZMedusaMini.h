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

namespace gadgeteering {
	namespace mainboards {
		class fez_medusa_mini : public base_mainboard {
			void create_sockets();

			socket sockets[11];
			bool serial_began;
			bool spi_began;

			software_i2c* i2c0;
			software_i2c* i2c1;
			software_i2c* i2c2;

			public:
				struct pins
				{
					static const cpu_pin IO0 = 0;
					static const cpu_pin IO1 = 1;
					static const cpu_pin IO2 = 2;
					static const cpu_pin IO3 = 3;
					static const cpu_pin IO4 = 4;
					static const cpu_pin IO5 = 5;
					static const cpu_pin IO6 = 6;
					static const cpu_pin IO7 = 7;
					static const cpu_pin IO8 = 8;
					static const cpu_pin IO9 = 9;
					static const cpu_pin IO10 = 10;
					static const cpu_pin IO11 = 11;
					static const cpu_pin IO12 = 12;
					static const cpu_pin IO13 = 13;
					static const cpu_pin AD0 = 14;
					static const cpu_pin AD1 = 15;
					static const cpu_pin AD2 = 16;
					static const cpu_pin AD3 = 17;
					static const cpu_pin AD4 = 18;
					static const cpu_pin AD5 = 19;
				};

				fez_medusa_mini();
				virtual ~fez_medusa_mini();

				virtual const socket& get_socket(unsigned char number);

				virtual void set_debug_led(bool state);

				virtual void set_io_mode(cpu_pin pin, io_mode new_io_mode, resistor_mode new_resistor_mode);
				virtual void write_digital(cpu_pin pin, bool value);
				virtual bool read_digital(cpu_pin pin);
				virtual void write_analog(analog_channel channel, double voltage);
				virtual double read_analog(analog_channel channel);
				virtual void set_pwm(pwm_channel channel, double duty_cycle, double frequency);

				void set_pwm(cpu_pin pin, double duty_cycle, double frequency, double duration); //socket 2 only

				virtual void spi_read_write(spi_channel channel, const unsigned char* write_buffer, unsigned char* read_buffer, unsigned int count, spi_configuration& config, bool deselect_after);
				virtual bool i2c_write(i2c_channel channel, const unsigned char* buffer, unsigned int length, bool send_start, bool send_stop);
				virtual bool i2c_read(i2c_channel channel, unsigned char* buffer, unsigned int length, bool send_start, bool send_stop);
				virtual unsigned int serial_write(serial_channel  channel, const unsigned char* buffer, unsigned int count, serial_configuration& config);
				virtual unsigned int serial_read(serial_channel  channel, unsigned char* buffer, unsigned int count, serial_configuration& config);
				virtual unsigned int serial_available(serial_channel channel);
		};

	}
}
