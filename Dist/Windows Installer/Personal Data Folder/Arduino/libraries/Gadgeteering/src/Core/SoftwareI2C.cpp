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

#include "SoftwareI2C.h"

#include "Mainboard.h"
#include "System.h"

using namespace gadgeteering;

software_i2c::software_i2c(cpu_pin sda, cpu_pin scl, bool use_resistors)
{
	this->use_resistors = use_resistors;
	this->start = false;
	this->scl = scl;
	this->sda = sda;
}

void software_i2c::clear_scl()
{
	mainboard->set_io_mode(this->scl, io_modes::DIGITAL_OUTPUT, resistor_modes::NONE);
	mainboard->write_digital(this->scl, false);
}

bool software_i2c::read_scl()
{
	mainboard->set_io_mode(this->scl, io_modes::DIGITAL_INPUT, this->use_resistors ? resistor_modes::PULL_UP : resistor_modes::FLOATING);
	return mainboard->read_digital(this->scl);
}

void software_i2c::release_scl()
{
	mainboard->set_io_mode(this->scl, io_modes::DIGITAL_INPUT, this->use_resistors ? resistor_modes::PULL_UP : resistor_modes::FLOATING);
}

void software_i2c::clear_sda()
{
	mainboard->set_io_mode(this->sda, io_modes::DIGITAL_OUTPUT, resistor_modes::NONE);
	mainboard->write_digital(this->sda, false);
}

bool software_i2c::read_sda()
{
	mainboard->set_io_mode(this->sda, io_modes::DIGITAL_INPUT, this->use_resistors ? resistor_modes::PULL_UP : resistor_modes::FLOATING);
	return mainboard->read_digital(this->sda);
}

void software_i2c::release_sda()
{
	mainboard->set_io_mode(this->sda, io_modes::DIGITAL_INPUT, this->use_resistors ? resistor_modes::PULL_UP : resistor_modes::FLOATING);
}

void software_i2c::wait_scl()
{
	unsigned long end = system::time_elapsed() + 5000;
	while (!this->read_scl() && system::time_elapsed() < end)
		;
}

void software_i2c::delay()
{

}

bool software_i2c::write_bit(bool bit)
{
	if (bit)
		this->release_sda();
	else
		this->clear_sda();

	this->delay();

	this->wait_scl();

	if (bit && !this->read_sda())
		return false;

	this->delay();

	this->clear_scl();

	return true;
}

bool software_i2c::read_bit()
{
	this->release_sda();

	this->delay();
	
	this->wait_scl();

	bool bit = this->read_sda();

	this->delay();

	this->clear_scl();

	return bit;
}

bool software_i2c::send_start_condition()
{
	if (this->start)
	{
		this->release_sda();

		this->delay();
		
		this->wait_scl();
	}

	if (!this->read_sda())
		return false;

	this->clear_sda();

	this->delay();

	this->clear_scl();

	this->start = true;

	return true;
}

bool software_i2c::send_stop_condition()
{
	this->clear_sda();

	this->delay();

	this->wait_scl();

	if (!this->read_sda())
		return false;

	this->delay();

	this->start = false;

	return true;
}

bool software_i2c::transmit(bool send_start, bool send_stop, unsigned char data)
{
	if (send_start)
		this->send_start_condition();

	for (unsigned char bit = 0; bit < 8; bit++)
	{
		this->write_bit((data & 0x80) != 0);

		data <<= 1;
	}

	bool nack = this->read_bit();

	if (send_stop)
		this->send_stop_condition();

	return nack;
}

unsigned char software_i2c::receive(bool send_ack, bool send_stop_condition)
{
	unsigned char d = 0;
	
	for (unsigned char bit = 0; bit < 8; bit++)
		d = (d << 1) | (this->read_bit() ? 1 : 0);

	this->write_bit(!send_ack);

	if (send_stop_condition)
		this->send_stop_condition();

	return d;
}

bool software_i2c::write(unsigned char address, const unsigned char* buffer, unsigned int length, bool send_start, bool send_stop)
{
	bool result = true;

	address <<= 1;

	if (!this->transmit(send_start, length == 0, address))
		result = false;

	for (unsigned int i = 0; i < length; i++)
		if (!this->transmit(false, i == length - 1 ? send_stop : false, buffer[i]))
			result = false;

	return result;
}

bool software_i2c::read(unsigned char address, unsigned char* buffer, unsigned int length, bool send_start, bool send_stop)
{
	bool result = true;

	address <<= 1;
	address |= 1;

	if (!this->transmit(send_start, length == 0, address))
		result = false;

	for (unsigned int i = 0; i < length; i++)
		buffer[i] = this->receive(i < length - 1, i == length - 1 ? send_stop : false);

	return result;
}
