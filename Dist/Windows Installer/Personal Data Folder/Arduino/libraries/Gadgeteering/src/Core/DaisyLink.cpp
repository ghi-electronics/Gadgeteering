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

#include "DaisyLink.h"

#include "System.h"
#include "Mainboard.h"

using namespace gadgeteering;
using namespace gadgeteering::interfaces;
using namespace gadgeteering::daisy_link;

bus::bus_node* bus::bus_list = NULL;

bus::bus(const socket& sock) : sock(sock), i2c(sock.pins[bus::SDA_PIN], sock.pins[bus::SCL_PIN], bus::DEFAULT_I2C_ADDRESS, false), neighbor_bus(sock, bus::DAISYLINK_PIN)
{
	this->module_count = 0;
	this->reference_count = 0;
	this->next_address = bus::START_ADDRESS;

	this->initialize();
}

void bus::initialize()
{
	this->neighbor_bus.write(false);
	system::sleep(2);
	this->neighbor_bus.set_resistor_mode(resistor_modes::FLOATING);
	this->neighbor_bus.read();

	unsigned char next_address = bus::START_ADDRESS;
	while (true)
	{
		if (this->module_count > 0)
		{
			this->i2c.address = next_address - 1;
			this->i2c.write_register(bus::registers::CONFIG, 0x00);
			this->i2c.address = bus::DEFAULT_I2C_ADDRESS;
		}

		if (this->i2c.read_register(bus::registers::DAISYLINK_VERSION) != 0x04)
			break;

		this->i2c.write_register(bus::registers::ADDRESS, next_address);

		this->module_count++;
		next_address++;

		system::sleep(2);
	}
}

unsigned int bus::get_length_of_chain()
{
	return this->module_count;
}

void bus::get_module_parameters(unsigned int position, unsigned char& manufacturer, unsigned char& type, unsigned char& version)
{
	unsigned char buffer[6];
	this->i2c.address = position + bus::START_ADDRESS;
	this->i2c.read_registers(0x00, buffer, 6);
	this->i2c.address = bus::DEFAULT_I2C_ADDRESS;
	manufacturer = buffer[bus::registers::MANUFACTURER];
	type = buffer[bus::registers::MODULE_TYPE];
	version = buffer[bus::registers::MODULE_VERSION];
}

unsigned char bus::get_version(unsigned int position)
{
	this->i2c.address = position + bus::START_ADDRESS;
	unsigned char version = this->i2c.read_register(bus::registers::DAISYLINK_VERSION);
	this->i2c.address = bus::DEFAULT_I2C_ADDRESS;
	return version;
}

bus& bus::get_bus(unsigned char socket_number)
{
	bus::bus_node* current = bus::bus_list;
	bus::bus_node* last = NULL;

	while (current)
	{
		if (current->data->sock.number == socket_number)
		{
			current->data->reference_count++;
			return *current->data;
		}

		last = current;
		current = current->next;
	}

	const socket& sock = mainboard->get_socket(socket_number);
	bus* new_bus = new bus(sock);

	if (bus::bus_list == NULL)
	{
		bus::bus_list = new bus::bus_node();
		bus::bus_list->next = NULL;
		bus::bus_list->data = new_bus;
	}
	else
	{
		last->next = new bus::bus_node();
		last->next->next = NULL;
		last->next->data = new_bus;
	}

	new_bus->reference_count++;
	return *new_bus;
}

void bus::release_bus(unsigned char socket_number)
{
	bus::bus_node* current = bus::bus_list;
	bus::bus_node* prev = NULL;

	while (current)
	{
		if (current->data->sock.number == socket_number)
		{
			if (--current->data->reference_count == 0)
			{
				if (current == bus::bus_list)
				{
					bus::bus_list = current->next;
				}
				else if (prev)
				{
					prev->next = current->next;
				}

				delete current->data;
				delete current;

				return;
			}
		}

		prev = current;
		current = current->next;
	}
}

module::module(unsigned char socket_number, unsigned char manufacturer, unsigned char type, unsigned char min_version_supported, unsigned char max_version_supported) : parent_bus(bus::get_bus(socket_number)), i2c(this->parent_bus.sock.pins[bus::SDA_PIN], this->parent_bus.sock.pins[bus::SCL_PIN], this->parent_bus.next_address++, false)
{
	this->socket_number = socket_number;
	this->position_on_chain = this->i2c.address - bus::START_ADDRESS;
	this->parent_bus.get_module_parameters(this->position_on_chain, this->manufacturer, this->type, this->version);
	this->daisy_link_version = this->parent_bus.get_version(this->position_on_chain);
	this->length_of_chain = this->parent_bus.get_length_of_chain();

	if (this->parent_bus.module_count == 0)
		panic_specific(errors::MODULE_ERROR, 1);

	if (this->parent_bus.reference_count > this->parent_bus.module_count)
		panic_specific(errors::MODULE_ERROR, 2);

	if (this->daisy_link_version != module::VERSION_IMPLEMENTED)
		panic_specific(errors::MODULE_ERROR, 3);

	if (manufacturer != this->manufacturer)
		panic_specific(errors::MODULE_ERROR, 4);

	if (type != this->type)
		panic_specific(errors::MODULE_ERROR, 5);

	if (this->version < min_version_supported || this->version > max_version_supported)
		panic_specific(errors::MODULE_ERROR, 6);
}

module::~module()
{

}

unsigned char module::read_register(unsigned char address)
{
	return this->i2c.read_register(address);
}

void module::write_register(unsigned char address, unsigned char value)
{
	this->i2c.write_register(address, value);
}

void module::read_registers(unsigned char address, unsigned char* buffer, unsigned int length)
{
	this->i2c.read_registers(address, buffer, length);
}

void module::write_registers(unsigned char address, const unsigned char* buffer, unsigned int length)
{
	this->i2c.write_registers(address, buffer, length);
}

void module::write_read(const unsigned char* write_buffer, unsigned int write_length, unsigned char* read_buffer, unsigned int read_length)
{
	this->i2c.write_read(write_buffer, write_length, read_buffer, read_length);
}

void module::write(const unsigned char* write_buffer, unsigned int write_length)
{
	this->i2c.write(write_buffer, write_length);
}

void module::read(unsigned char* read_buffer, unsigned int read_length)
{
	this->i2c.read(read_buffer, read_length);
}

unsigned int module::get_position_on_chain() const
{
	return this->position_on_chain;
}

unsigned int module::get_length_of_chain() const
{
	return this->length_of_chain;
}

unsigned int module::get_socket_number() const
{
	return this->socket_number;
}

bool module::has_signaled_interrupt()
{
	unsigned char reg = this->i2c.read_register(bus::registers::CONFIG);
	bool interrupted = (reg & 0x80) != 0;
	
	if (interrupted)
	{
		reg &= 0x7F;
		this->i2c.write_register(bus::registers::CONFIG, reg);
	}

	return interrupted;
}