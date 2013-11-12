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

#include "I2CDevice.h"
#include "I2CBus.h"

using namespace Gadgeteering;
using namespace Gadgeteering::Interfaces;

I2CDevice::I2CDevice(Interfaces::I2CBus* bus, unsigned char address) {
	this->bus = bus;
	this->address = address << 1;
}

I2CDevice::~I2CDevice() {

}

unsigned int I2CDevice::write(const unsigned char* buffer, unsigned int count, bool sendStop)
{
	this->bus->write(&this->address, 1, true, false);
	return this->bus->write(buffer, count, false, sendStop);
}

unsigned int I2CDevice::read(unsigned char* buffer, unsigned int count, bool sendStop)
{
	this->bus->write(&this->address, 1 | 1, true, false);
	return this->bus->read(buffer, count, false, sendStop);
}

bool I2CDevice::writeRead(const unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength, unsigned int* numWritten, unsigned int* numRead)
{
	bool w = true, r = true;

	this->bus->write(&this->address, 1, true, false);
	w = this->bus->write(writeBuffer, writeLength, false, readLength == 0);
	if (readLength)
	{
		this->address |= 1;
		this->bus->write(&this->address, 1, true, false);
		r = this->bus->read(readBuffer, readLength, false, true);
		this->address &= ~1;
	}
	return w && r;
}
				
bool I2CDevice::writeRegister(unsigned char address, unsigned char value) {
	unsigned char data[2] = {address, value};
	return this->write(data, 2, true) == 2;
}

bool I2CDevice::writeRegisters(unsigned char startAddress, unsigned char* values, unsigned int count) {
	unsigned char* data = new unsigned char[count + 1];
	data[0] = startAddress;
	for (unsigned int i = 0; i < count; i++)
		data[i + 1] = values[i];

	return this->write(data, count + 1, true) == count + 1;
}

unsigned char I2CDevice::readRegister(unsigned char address) {
	unsigned char value;
	unsigned int a, b;
	this->writeRead(&address, 1, &value, 1, &a, &b);
	return value;
}

bool I2CDevice::readRegisters(unsigned char startAddress, unsigned char* values, unsigned int count) {
	unsigned int a, b;
	return this->writeRead(&startAddress, 1, values, count, &a, &b);
}