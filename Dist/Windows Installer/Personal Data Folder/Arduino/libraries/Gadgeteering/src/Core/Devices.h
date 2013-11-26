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
#include "Socket.h"
#include "SoftwareI2C.h"

namespace gadgeteering
{
	namespace devices
	{
		class i2c
		{
			i2c_channel channel;
			software_i2c* soft_i2c;

			public:
				unsigned char address;

				i2c(const socket& sock, unsigned char address, bool uses_hardware_i2c = true);
				i2c(i2c_channel channel, unsigned char address);
				i2c(cpu_pin sda, cpu_pin scl, unsigned char address, bool use_resistors = true);
				~i2c();

				bool write(const unsigned char* buffer, unsigned int length, bool send_start = true, bool send_stop = true);
				bool read(unsigned char* buffer, unsigned int length, bool send_start = true, bool send_stop = true);

				bool write_read(const unsigned char* write_buffer, unsigned int write_length, unsigned char* read_buffer, unsigned int read_length);

				bool write_register(unsigned char address, unsigned char value);
				bool write_registers(unsigned char start_address, const unsigned char* values, unsigned int length);
				unsigned char read_register(unsigned char address);
				bool read_registers(unsigned char start_address, unsigned char* values, unsigned int length);
		};

		class spi
		{
			spi_channel channel;

			public:
				spi_configuration config;

				spi(const socket& spi_socket, spi_configuration configuration);
				spi(const socket& spi_socket, spi_configuration configuration, const socket& cs_socket, socket_pin_number cs_pin_number);
				spi(spi_channel channel, spi_configuration configuration);
				spi(spi_channel channel, spi_configuration configuration, const socket& cs_socket, socket_pin_number cs_pin_number);
				~spi();

				//Clocks in one char and clocks out one char at the same time. If deselect_after is true, the CS line is set to logic low after the transmission, otherwise it remains logic high.
				unsigned char write_read_byte(unsigned char value, bool deselect_after = true);

				//Clocks length bytes in and out at the same time to and from the receive and send buffer respectively.
				void write_read(const unsigned char* write_buffer, unsigned char* read_buffer, unsigned int length, bool deselect_after = true);

				//Clocks write_length bytes from write_buffer out while ignoring the received bytes and then clocks read_length bytes into the read_buffer while clocking 0's out.
				void write_then_read(const unsigned char* write_buffer, unsigned char* read_buffer, unsigned int write_length, unsigned int read_length, bool deselect_after = true);

				//Clocks length bytes out from the buffer while ignoring the bytes clocked in.
				void write(const unsigned char* buffer, unsigned int length, bool deselect_after = true);

				//Clocks length bytes in while clocking 0's out.
				void read(unsigned char* buffer, unsigned int length, bool deselect_after = true);
		};

		class serial
		{
			serial_channel channel;

			public:
				serial_configuration config;

				serial(const socket& sock, serial_configuration configuration);
				serial(serial_channel channel, serial_configuration configuration);
				~serial();

				void write(const unsigned char* buffer, unsigned int length);
				void write(const char* buffer, unsigned int length = 0);
				unsigned int read(unsigned char* buffer, unsigned int length);
				unsigned int available();
		};
	}
}
