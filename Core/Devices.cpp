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

#include "Devices.h"
#include "Mainboard.h"

using namespace gadgeteering;
using namespace gadgeteering::devices;

i2c::i2c(socket& socket, unsigned char address)
{
	socket.ensure_type(socket::types::I);

	this->channel = socket.i2c;
	this->w_address = address << 1;
	this->r_address = (address << 1) | 1;
}

bool i2c::write(const unsigned char* buffer, unsigned int length, bool send_start, bool send_stop)
{
	mainboard->i2c_write(this->channel, &this->w_address, 1, send_start, false);
	return mainboard->i2c_write(this->channel, buffer, length, false, send_stop);
}

bool i2c::read(unsigned char* buffer, unsigned int length, bool send_start, bool send_stop)
{
	mainboard->i2c_write(this->channel, &this->r_address, 1, send_start, false);
	return mainboard->i2c_read(this->channel, buffer, length, false, send_stop);
}

bool i2c::write_read(const unsigned char* write_buffer, unsigned int write_length, unsigned char* read_buffer, unsigned int read_length)
{
	bool w = true, r = true;

	w = this->write(write_buffer, write_length, true, read_length == 0);

	if (read_length != 0)
		r = this->read(read_buffer, read_length, true, true);
	
	return w && r;
}

bool i2c::write_register(unsigned char address, unsigned char value)
{
	unsigned char data[2] = { address, value };
	return this->write(data, 2);
}

bool i2c::write_registers(unsigned char start_address, unsigned char* values, unsigned int length)
{
	unsigned char* data = new unsigned char[length + 1];
	data[0] = start_address;
	for (unsigned int i = 0; i < length; i++)
		data[i + 1] = values[i];

	bool result = this->write(data, length + 1);
	delete[] data;
	return result;
}

unsigned char i2c::read_register(unsigned char address)
{
	unsigned char value;
	this->write_read(&address, 1, &value, 1);
	return value;
}

bool i2c::read_registers(unsigned char start_address, unsigned char* values, unsigned int length)
{
	return this->write_read(&start_address, 1, values, length);
}

spi::spi(socket& socket, spi_configuration configuration)
{
	socket.ensure_type(socket::types::S);

	this->config = configuration;
	this->channel = socket.spi;
}

spi::spi(socket& spi_socket, spi_configuration configuration, socket& cs_socket, socket::pin cs_pin_number)
{
	spi_socket.ensure_type(socket::types::S);

	this->config = configuration;
	this->channel = spi_socket.spi;
	this->config.chip_select = cs_socket.pins[cs_pin_number];
}

unsigned char spi::write_read_byte(unsigned char value, bool deselect_after) 
{ 
	unsigned char received;
	this->write_read(&value, &received, 1, deselect_after);
	return received;
}

void spi::write_read(const unsigned char* write_buffer, unsigned char* read_buffer, unsigned int length, bool deselect_after) 
{
	mainboard->spi_read_write(this->channel, write_buffer, read_buffer, length, this->config, deselect_after);
}

void spi::write_then_read(const unsigned char* write_buffer, unsigned char* read_buffer, unsigned int write_length, unsigned int read_length, bool deselect_after) 
{
	this->write_read(write_buffer, NULL, write_length, false);
	this->write_read(NULL, read_buffer, read_length, deselect_after);
}

void spi::write(const unsigned char* buffer, unsigned int length, bool deselect_after) 
{
	this->write_read(buffer, NULL, length, deselect_after);
}

void spi::read(unsigned char* buffer, unsigned int length, bool deselect_after) 
{
	this->write_read(NULL, buffer, length, deselect_after);
}

serial::serial(socket& socket, serial_configuration configuration)
{
	socket.ensure_type(socket::types::U);

	this->config = configuration;
	this->channel = socket.serial;
}

void serial::write(const unsigned char* buffer, unsigned int length) 
{
	mainboard->serial_write(this->channel, buffer, length, this->config);
}

void serial::write(const char* buffer, unsigned int length) 
{ 
	this->write(reinterpret_cast<const unsigned char*>(buffer), length); 
}

unsigned int serial::read(unsigned char* buffer, unsigned int length) 
{ 
	return mainboard->serial_read(this->channel, buffer, length, this->config);
}

unsigned int serial::available() 
{ 
	return mainboard->serial_available(this->channel);
}