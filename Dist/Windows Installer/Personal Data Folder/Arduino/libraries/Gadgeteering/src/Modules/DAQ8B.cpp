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

#include "DAQ8B.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;

daq_8b::daq_8b(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::S)), cs(this->sock, 6, true), miso(this->sock, 8, resistor_modes::FLOATING), mosi(this->sock, 7, false), clock(this->sock, 9, false)
{

}

unsigned short daq_8b::spi_write_read(unsigned short data)
{
	unsigned short read = 0;

	this->cs.write(false);

	for (unsigned short j = 0, mask = 0x8000; j < 16; j++, mask >>= 1)
	{
		this->clock.write(false);

		this->mosi.write((data & mask) != 0);

		this->clock.write(true);

		if (this->miso.read())
			read |= mask;
	}

	this->mosi.write(true);
	this->clock.write(false);
	this->cs.write(true);

	return read;
}

double daq_8b::get_reading(channel c)
{
	this->spi_write_read(static_cast<unsigned short>(0xF124 | c));
	return 4.096 * static_cast<double>(this->spi_write_read(0x0000)) / 65535.0;
}
