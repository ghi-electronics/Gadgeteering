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

#include "RelayISOx16.h"

using namespace gadgeteering;
using namespace gadgeteering::interfaces;
using namespace gadgeteering::modules;

relay_iso_x16::relay_iso_x16(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::Y)), enable(this->sock, 3, true), clear(this->sock, 4, true), latch(this->sock, 5), data(this->sock, 7, true), clock(this->sock, 9)
{
	this->disable_all_relays();

	this->enable_output();
}

void relay_iso_x16::disable_all_relays()
{
	this->reg_data = 0x0000;

	this->write_data();
}

void relay_iso_x16::enable_all_relays()
{
	this->reg_data = 0xFFFF;

	this->write_data();
}

void relay_iso_x16::enable_output()
{
	this->enable.write(false);
}

void relay_iso_x16::disable_output()
{
	this->enable.write(true);
}

void relay_iso_x16::enable_relays(unsigned short relay_mask)
{
	this->reg_data |= relay_mask;

	this->write_data();
}

void relay_iso_x16::disable_relays(unsigned short relay_mask)
{
	this->reg_data &= ~relay_mask;

	this->write_data();
}

void relay_iso_x16::write_data()
{
	unsigned short reg = this->reg_data;

	for (int i = 0; i < 16; i++)
	{
		this->data.write((reg & 0x1) == 1);
		this->data.write(true);

		this->clock.write(true);
		system::sleep_micro(3);
		this->clock.write(false);

		reg >>= 1;
		system::sleep_micro(57);
	}

	this->latch.write(true);
	this->latch.write(false);
}
