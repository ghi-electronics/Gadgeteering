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

#include "TouchL12.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

TouchL12::TouchL12(unsigned char socketNumber)
{
	socket* t_socket = mainboard->getSocket(socketNumber);
	t_socket->ensureTypeIsSupported(socket::types::I);

	this->reset = new digital_output(socket, socket::pins::Six, true);

	this->Reset();

	this->device = t_socket->getI2CDevice(TouchL12::I2C_ADDRESS);

	this->ConfigureSPM();
}

TouchL12::~TouchL12()
{
	delete this->reset;
	delete this->device;
}

void TouchL12::Reset()
{
	System::Sleep(100);
	this->reset->write(false);
	System::Sleep(100);
	this->reset->write(true);
	System::Sleep(100);
}

void TouchL12::ConfigureSPM()
{
	//SPM must be written in 8 byte segments, so that is why we have the extra stuff below.

	//0x4 is cap sensitivity mode, 0xFF to 0x55 are the cap type modes, 0x70 is cap 0 and 1 sensitivty, the last 3 0's set cap 2-7 sensitivity.
	unsigned char d1[9] = { 0x0, 0x1, 0xFF, 0xFF, 0xFF, 0x70, 0x0, 0x0, 0x0 };
	this->WriteSPM(0x9, d1);

	//0x74 is to enable proximity sensing and the remaining values are the default reserved values that we cannot change
	//unsigned char d2[9] = { 0x0, 0x74, 0x10, 0x45, 0x2, 0xFF, 0xFF, 0xFF, 0xD5 };
	//this->WriteSPM(0x70, d2);
}

void TouchL12::WriteSPM(unsigned char address, const unsigned char data[9])
{
	this->device->writeRegister(0x0D, 0x10);
	this->device->writeRegister(0x0E, address);

	this->device->write(data, 9); //needs to begin with a 0 representing the I2C address of 0.

	this->device->writeRegister(0x0D, 0x00);
}

bool TouchL12::IsSliderPressed()
{
	return (this->device->readRegister(TouchL12::CAP_STAT_MSB) & 0x10) != 0;
}

double TouchL12::GetSliderPosition()
{
	unsigned char whlLsb = this->device->readRegister(TouchL12::WHL_POS_LSB);
	unsigned char whlMsb = this->device->readRegister(TouchL12::WHL_POS_MSB);
	double wheelPosition = (double)((whlMsb << 8) + whlLsb);
	return wheelPosition / (10.0 * TouchL12::WHEELS);
}

TouchL12::Direction TouchL12::GetSliderDirection()
{
	return (this->device->readRegister(TouchL12::CAP_STAT_MSB) & 0x40) != 0 ? Directions::Right : Directions::Left;
}
