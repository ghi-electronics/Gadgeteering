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

thermocouple::thermocouple(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::X)), miso(this->sock, 3, resistor_modes::PULL_UP), clk(this->sock, 4), cs(this->sock, 5)
{

}

unsigned long thermocouple::read_data()
{
	long bit_count;
	unsigned long data = 0;

	this->cs.write(false);
	{
		for (bit_count = 31; bit_count >= 0; bit_count--)
		{
			this->clk.write(true);
			system::sleep(1);
			if (this->miso.read())
			{
				data |= static_cast<unsigned long>(1L << static_cast<unsigned long>(bit_count));
			}

			this->clk.write(false);
			system::sleep(1);
		}
	}
	this->cs.write(true);

	return data;
}

short thermocouple::get_external_temp_celsius()
{
	return (read_data() >> 20) & 0xFFF;
}

short thermocouple::get_external_temp_fahrenheit()
{
	return static_cast<short>((get_external_temp_celsius() * 1.8) + 32);
}

unsigned char thermocouple::get_internal_temp_celsius()
{
	unsigned long value = this->read_data();
	return static_cast<unsigned char>((value >> 8) & 0xFF); // get byte 2
}
