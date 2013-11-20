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

#include "MaxO.h"

#include <string.h>
#include <stdlib.h>

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

max_o::max_o(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::S)), enable(this->sock, 3, false), clr(this->sock, 4, true), spi(this->sock.spi, spi_configuration(false, 0, 0, false, true, 1000))
{
	this->resized = false;
	this->num_boards = 0;
	this->data = NULL;
}

max_o::~max_o()
{
	if (this->data)
		delete[] this->data;
}

void max_o::set_number_of_boards(unsigned int boards)
{
	if (!this->resized)
	{
		this->num_boards = boards;
		this->length = this->num_boards * 4;
		this->data = new unsigned char[this->num_boards * 4];
		this->resized = true;
	}
	else
	{
		panic(errors::MODULE_ERROR);
	}
}

unsigned int max_o::get_array_size()
{
	return this->length;
}

void max_o::clear()
{
	if (!resized)
		panic(errors::MODULE_ERROR);

	this->enable.write(true);
	this->clr.write(false);

	system::sleep(10);
	unsigned char clear = 0x00;
	this->spi.write(&clear, 1, true);

	this->clr.write(true);
	this->enable.write(false);

	for (unsigned int i = 0; i < this->length; i++)
		this->data[i] = 0x0;
}

void max_o::write_array(unsigned char* arr, unsigned int length)
{
	if (!resized)
		panic(errors::MODULE_ERROR);

	if (length != this->length)
		panic(errors::MODULE_ERROR);

	this->enable.write(true);

	unsigned char* reversed = new unsigned char[length];
	memcpy(reversed, arr, length);

	for (unsigned int i = 0; i < length; i++)
		reversed[i] = arr[this->length - i - 1];

	this->spi.write(reversed, length, true);
	memcpy(this->data, arr, length);
	delete[] reversed;

	this->enable.write(false);
}

void max_o::write_pin(unsigned int board, unsigned int pin, bool value)
{
	if (!resized)
		panic(errors::MODULE_ERROR);

	// check to see if the pin is inside our range
	unsigned int length = board * 4;
	unsigned int position = ((board - 1) * 4) + pin;

	if (length > this->length)
		panic(errors::MODULE_ERROR);

	// make a "dummy" to turn our pin on or off
	unsigned char* dummy = new unsigned char[this->length];

	memcpy(dummy, this->data, this->length);

	// find exact bit position
	int block_pos = /*dummy.Length - */(((board - 1) * 4) + (pin / 8));
	//block_pos--;
	if (value)
	{
		dummy[block_pos] = (unsigned char)(this->data[block_pos] | (1 << ((pin % 8) /*- 1*/)));
		write_array(dummy, this->length);
	}
	else
	{
		dummy[block_pos] = (unsigned char)(this->data[block_pos] & ~(1 << ((pin % 8) /*- 1*/)));
		write_array(dummy, this->length);
	}

	delete[] dummy;
}

unsigned char* max_o::read()
{
	if (!resized)
		panic(errors::MODULE_ERROR);

	return this->data;
}

void max_o::enable_outputs()
{
	if (!resized)
		panic(errors::MODULE_ERROR);

	this->enable.write(false);
}

void max_o::disable_outputs()
{
	if (!resized)
		panic(errors::MODULE_ERROR);

	this->enable.write(true);
}
