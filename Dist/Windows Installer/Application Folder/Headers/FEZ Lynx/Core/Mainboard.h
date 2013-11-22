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

#include "Socket.h"
#include "Types.h"

namespace gadgeteering 
{
	class base_mainboard
	{
		protected:
			base_mainboard(double max_analog_voltage);
			virtual ~base_mainboard() = 0;

		public:
			const double max_analog_voltage;

			virtual socket& register_socket(socket s) = 0;
			virtual const socket& get_socket(unsigned char number) = 0;
			const socket& get_socket(unsigned char number, socket::type ensure_type);

			virtual void set_debug_led(bool state) = 0;

			virtual void set_io_mode(cpu_pin pin, io_mode new_io_mode, resistor_mode new_resistor_mode) = 0;
			virtual void write_digital(cpu_pin pin, bool value) = 0;
			virtual bool read_digital(cpu_pin pin) = 0;
			virtual void write_analog(analog_out_channel channel, double voltage_proportion) = 0;
			virtual double read_analog(analog_channel channel) = 0;
			virtual void set_pwm(pwm_channel channel, double duty_cycle, double frequency) = 0;

			virtual void spi_begin(spi_channel channel, spi_configuration& config) = 0;
			virtual void spi_end(spi_channel channel, spi_configuration& config) = 0;
			virtual void spi_read_write(spi_channel channel, const unsigned char* write_buffer, unsigned char* read_buffer, unsigned int count, spi_configuration& config, bool deselect_after) = 0;

			virtual void i2c_begin(i2c_channel channel) = 0;
			virtual void i2c_end(i2c_channel channel) = 0;
			virtual bool i2c_write(i2c_channel channel, unsigned char address, const unsigned char* buffer, unsigned int length, bool send_start, bool send_stop) = 0;
			virtual bool i2c_read(i2c_channel channel, unsigned char address, unsigned char* buffer, unsigned int length, bool send_start, bool send_stop) = 0;

			virtual void serial_begin(serial_channel channel, serial_configuration& config) = 0;
			virtual void serial_end(serial_channel channel, serial_configuration& config) = 0;
			virtual unsigned int serial_write(serial_channel channel, const unsigned char* buffer, unsigned int count, serial_configuration& config) = 0;
			virtual unsigned int serial_read(serial_channel channel, unsigned char* buffer, unsigned int count, serial_configuration& config) = 0;
			virtual unsigned int serial_available(serial_channel channel) = 0;
	};

	extern base_mainboard* mainboard;
}
