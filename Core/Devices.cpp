/*
Copyright 2013 Gadgeteering Electronics LLC

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

using namespace Gadgeteering;
using namespace Gadgeteering::Devices;

i2c::i2c(Socket& socket, unsigned char address)
{
	socket.ensureTypeIsSupported(Socket::Types::I);

	this->module = socket.i2c_port;
	this->w_address = address << 1;
	this->r_address = (address << 1) | 1;
}

bool i2c::write(const unsigned char* buffer, unsigned int count, bool sendStart, bool sendStop)
{
	mainboard->i2c_write(this->module, &this->w_address, 1, sendStart, false);
	return mainboard->i2c_write(this->module, buffer, count, false, sendStop);
}

bool i2c::read(unsigned char* buffer, unsigned int count, bool sendStart, bool sendStop)
{
	mainboard->i2c_write(this->module, &this->r_address, 1, sendStart, false);
	return mainboard->i2c_read(this->module, buffer, count, false, sendStop);
}

bool i2c::writeRead(const unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength)
{
	bool w = true, r = true;

	w = this->write(writeBuffer, writeLength, true, readLength == 0);

	if (readLength != 0)
		r = this->read(readBuffer, readLength, true, true);
	
	return w && r;
}

bool i2c::writeRegister(unsigned char address, unsigned char value)
{
	unsigned char data[2] = { address, value };
	return this->write(data, 2);
}

bool i2c::writeRegisters(unsigned char startAddress, unsigned char* values, unsigned int count)
{
	unsigned char* data = new unsigned char[count + 1];
	data[0] = startAddress;
	for (unsigned int i = 0; i < count; i++)
		data[i + 1] = values[i];

	bool result = this->write(data, count + 1);
	delete[] data;
	return result;
}

unsigned char i2c::readRegister(unsigned char address)
{
	unsigned char value;
	this->writeRead(&address, 1, &value, 1);
	return value;
}

bool i2c::readRegisters(unsigned char startAddress, unsigned char* values, unsigned int count)
{
	return this->writeRead(&startAddress, 1, values, count);
}

spi::spi(Socket& socket, Socket& chipselect_socket, Socket::Pin chipselect_pin, SPIConfiguration configuration)
{
	socket.ensureTypeIsSupported(Socket::Types::S);

	this->config = configuration;
	this->module = socket.spi_port;
}

unsigned char spi::writeReadByte(unsigned char toSend, bool deselectChip) 
{ 
	unsigned char received;
	this->writeAndRead(&toSend, &received, 1, deselectChip);
	return received;
}

void spi::writeAndRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, bool deselectChip) 
{
	mainboard->spi_read_write(this->module, sendBuffer, receiveBuffer, count, this->config, deselectChip);
}

void spi::writeThenRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, bool deselectChip) 
{
	this->writeAndRead(sendBuffer, NULL, sendCount, false);
	this->writeAndRead(NULL, receiveBuffer, receiveCount, deselectChip);
}

void spi::write(const unsigned char* buffer, unsigned int count, bool deselectChip) 
{
	this->writeAndRead(buffer, NULL, count, deselectChip);
}

void spi::read(unsigned char* buffer, unsigned int count, bool deselectChip) 
{
	this->writeAndRead(NULL, buffer, count, deselectChip);
}

serial::serial(Socket& socket, serial_configuration configuration)
{
	socket.ensureTypeIsSupported(Socket::Types::U);

	this->config = configuration;
	this->module = socket.serial_port;
}

void serial::write(const unsigned char* buffer, unsigned int count) 
{
	mainboard->serial_write(this->module, buffer, count, this->config);
}

void serial::write(const char* buffer, unsigned int count) 
{ 
	this->write(reinterpret_cast<const unsigned char*>(buffer), count); 
}

unsigned int serial::read(unsigned char* buffer, unsigned int count) 
{ 
	return mainboard->serial_read(this->module, buffer, count, this->config);
}

unsigned int serial::available() 
{ 
	return mainboard->serial_available(this->module);
}