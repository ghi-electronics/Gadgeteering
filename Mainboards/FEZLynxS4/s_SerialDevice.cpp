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

#include "FEZLynxS4.h"

using namespace Gadgeteering;
using namespace Gadgeteering::Mainboards;

FEZLynxS4::SerialDevice::SerialDevice(CPUPin tx, CPUPin rx, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits, ftdi_channel& channel) : channel(channel), Interfaces::SerialDevice(tx, rx, baudRate, parity, stopBits, dataBits)
{
	this->config.baud_rate = baudRate;

	switch (parity)
	{
		case SerialDevice::Parity::EVEN: this->config.parity = FT_PARITY_EVEN;
		case SerialDevice::Parity::ODD: this->config.parity = FT_PARITY_ODD;
		case SerialDevice::Parity::MARK: this->config.parity = FT_PARITY_MARK;
		case SerialDevice::Parity::SPACE: this->config.parity = FT_PARITY_SPACE;
		case SerialDevice::Parity::NONE: this->config.parity = FT_PARITY_NONE;
		default: mainboard->panic(Exceptions::ERR_SERIAL_NOT_SUPPORTED, 0);
	}

	switch (stopBits)
	{
		case SerialDevice::StopBits::ONE: this->config.stop_bits = FT_STOP_BITS_1;
		case SerialDevice::StopBits::TWO: this->config.stop_bits = FT_STOP_BITS_2;
		default: mainboard->panic(Exceptions::ERR_SERIAL_NOT_SUPPORTED, 1);
	}

	switch (dataBits)
	{
		case 7: this->config.data_bits = FT_BITS_7;
		case 8: this->config.data_bits = FT_BITS_8;
		default: mainboard->panic(Exceptions::ERR_SERIAL_NOT_SUPPORTED, 2);
	}
}

FEZLynxS4::SerialDevice::~SerialDevice()
{

}

void FEZLynxS4::SerialDevice::open()
{
	this->channel.set_mode(ftdi_channel::modes::SERIAL);
}

void FEZLynxS4::SerialDevice::close()
{

}

void FEZLynxS4::SerialDevice::write(const unsigned char* buffer, unsigned int count)
{
	this->channel.serial_write(buffer, count, this->config);
}

unsigned int FEZLynxS4::SerialDevice::read(unsigned char* buffer, unsigned int count)
{
	return this->channel.serial_read(buffer, count, this->config);
}

unsigned int FEZLynxS4::SerialDevice::available()
{
	return this->channel.serial_available();
}
