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

#include "Thermocouple.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

Thermocouple::Thermocouple(unsigned char socketNumber)
{
	socket* t_socket = mainboard->getSocket(socketNumber);
	t_socket->ensureTypeIsSupported(socket::types::X);

	_miso = new digital_input(socket, socket::pins::Three, resistor_modes::PULL_UP);
	_clk = new digital_output(socket, socket::pins::Four, false);
	_cs = new digital_output(socket, socket::pins::Five, true);
}

Thermocouple::~Thermocouple()
{
	delete this->_miso;
	delete this->_clk;
	delete this->_cs;
}

unsigned long Thermocouple::ReadData()
{
	long bitCount;
	unsigned long data = 0;

	_cs->write(false);
	{
		for (bitCount = 31; bitCount >= 0; bitCount--)
{
			_clk->write(true);
			System::Sleep(1);
			if (_miso->read())
{
				data |= (unsigned long)(1L << (unsigned long)bitCount);
			}

			_clk->write(false);
			System::Sleep(1);
		}
	}
	_cs->write(true);

	return data;
}

short Thermocouple::GetExternalTemp_Celsius()
{
	return (ReadData() >> 20) & 0xFFF;
}

short Thermocouple::GetExternalTemp_Fahrenheit()
{
	return (short)((GetExternalTemp_Celsius() * 1.8) + 32);
}

unsigned char Thermocouple::GetInternalTemp_Celsius()
{
	unsigned long value = ReadData();
	return (unsigned char)((value >> 8) & 0xFF); // get byte 2
}
