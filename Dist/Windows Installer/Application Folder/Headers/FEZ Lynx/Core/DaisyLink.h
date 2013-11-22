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

#include "Interfaces.h"
#include "Devices.h"
#include "Socket.h"
#include "Types.h"

namespace gadgeteering
{
	namespace daisy_link
	{
		class bus
		{
			static const socket_pin_number DAISYLINK_PIN = 3;
			static const socket_pin_number SDA_PIN = 4;
			static const socket_pin_number SCL_PIN = 5;
			static const unsigned char DEFAULT_I2C_ADDRESS = 0x7F;
			static const unsigned char START_ADDRESS = 0x01;

			struct registers
			{
				static const unsigned char ADDRESS = 0;
				static const unsigned char CONFIG = 1;
				static const unsigned char DAISYLINK_VERSION = 2;
				static const unsigned char MODULE_TYPE = 3;
				static const unsigned char MODULE_VERSION = 4;
				static const unsigned char MANUFACTURER = 5;
			};

			struct bus_node
			{
				bus_node* next;
				bus* data;
			};

			static bus_node* bus_list;

			interfaces::digital_io neighbor_bus;
			devices::i2c i2c;
			const socket& sock;
			unsigned int module_count;
			unsigned int next_address;
			unsigned int reference_count;

			bus(const socket& sock);

			void initialize();
			unsigned int get_length_of_chain();
			void get_module_parameters(unsigned int position, unsigned char& manufacturer, unsigned char& type, unsigned char& version);

			friend class module;

			unsigned char get_version(unsigned int position);

			public:
				static bus& get_bus(unsigned char socket_number);
				static void release_bus(unsigned char socket_number);
		};

		class module
		{
			protected:
				bus& parent_bus;
				devices::i2c i2c;

				static const unsigned char REGISTER_OFFSET = 8;
				static const unsigned char VERSION_IMPLEMENTED = 4;

				unsigned char manufacturer;
				unsigned char type;
				unsigned char version;
				unsigned char daisy_link_version;

				unsigned int position_on_chain;
				unsigned int length_of_chain;
				unsigned int socket_number;

				module(unsigned char socket_number, unsigned char manufacturer, unsigned char type, unsigned char min_version_supported, unsigned char max_version_supported);
				virtual ~module();

				unsigned char read_register(unsigned char address);
				void write_register(unsigned char address, unsigned char value);
				void read_registers(unsigned char address, unsigned char* buffer, unsigned int length);
				void write_registers(unsigned char address, const unsigned char* buffer, unsigned int length);
				void write_read(const unsigned char* write_buffer, unsigned int write_length, unsigned char* read_buffer, unsigned int read_length);
				void write(const unsigned char* write_buffer, unsigned int write_length);
				void read(unsigned char* read_buffer, unsigned int read_length);

			public:
				unsigned int get_position_on_chain() const;
				unsigned int get_length_of_chain() const;
				unsigned int get_socket_number() const;
				bool has_signaled_interrupt();
		};
	}
}
