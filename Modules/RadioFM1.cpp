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

#include "RadioFM1.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

const double radio_fm1::MAX_CHANNEL = 107.5;
const double radio_fm1::MIN_CHANNEL = 87.5;
const double radio_fm1::INVALID_CHANNEL = -1.0;

radio_fm1::radio_fm1(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::Y)), i2c(this->sock.i2c, radio_fm1::I2C_ADDRESS), reset_pin(this->sock, 5), sen_pin(this->sock, 4, true)
{
	this->reset_pin.write(true);

	system::sleep(100);

	this->read_registers();
	this->registers[0x07] = 0x8100; //Enable the oscillator
	this->update_registers();

	system::sleep(500); //Wait for clock to settle - from AN230 page 9

	this->read_registers();
	this->registers[registers::POWERCFG] = 0x4001; //Enable the IC
	this->registers[registers::SYSCONFIG1] &= ~(1 << radio_fm1::BIT_RDS); //Disable RDS
	this->registers[registers::SYSCONFIG2] &= 0xFFCF; //Force 200kHz Channel spacing for USA
	this->registers[registers::SYSCONFIG2] &= 0xFFF0; //clear new_volume bits
	this->registers[registers::SYSCONFIG2] |= 0x000F; //Set new_volume to lowest
	this->update_registers();

	system::sleep(110); //Max powerup time, from datasheet page 13

	this->set_channel(radio_fm1::MIN_CHANNEL);
	this->set_volume(radio_fm1::MIN_VOLUME);
}

void radio_fm1::read_registers()
{
	unsigned char data[32];

	this->i2c.read(data, 32);

	for (int i = 0, x = 0xA; i < 12; i += 2, ++x)
		this->registers[x] = static_cast<unsigned short>((data[i] << 8) | (data[i + 1]));

	for (int i = 12, x = 0x0; i < 32; i += 2, ++x)
		this->registers[x] = static_cast<unsigned short>((data[i] << 8) | (data[i + 1]));
}

void radio_fm1::update_registers()
{
	unsigned char data[12];

	for (int x = 0x02, i = 0; x < 0x08; ++x, i += 2)
	{
		data[i] = static_cast<unsigned char>(this->registers[x] >> 8);
		data[i + 1] = static_cast<unsigned char>(this->registers[x] & 0x00FF);
	}
	this->i2c.write(data, 12);
}

void radio_fm1::increase_volume()
{
	this->set_volume(this->volume + 1);
}

void radio_fm1::decrease_volume()
{
	this->set_volume(this->volume - 1);
}

void radio_fm1::set_volume(unsigned short new_volume)
{
	if (new_volume > radio_fm1::MAX_VOLUME) new_volume = radio_fm1::MAX_VOLUME;

	this->read_registers();
	this->registers[registers::SYSCONFIG2] &= 0xFFF0; //clear new_volume bits
	this->registers[registers::SYSCONFIG2] |= new_volume; //Set new_volume to lowest
	this->update_registers();

	this->volume = new_volume;
}

unsigned short radio_fm1::get_volume()
{
	return this->volume;
}

void radio_fm1::set_channel(double new_channel)
{
	if (new_channel > radio_fm1::MAX_CHANNEL) new_channel = radio_fm1::MAX_CHANNEL;
	if (new_channel < radio_fm1::MIN_CHANNEL) new_channel = radio_fm1::MIN_CHANNEL;

	new_channel *= 10;
	new_channel -= 875;
	new_channel /= 2;

	this->read_registers();
	this->registers[registers::CHANNEL] &= 0xFE00; //clear out the Channel bits
	this->registers[registers::CHANNEL] |= (unsigned short)new_channel; //Mask in the new Channel
	this->registers[registers::CHANNEL] |= (1 << radio_fm1::BIT_TUNE); //Set the TUNE bit to start
	this->update_registers();

	//Poll to see if STC is set
	while (true)
	{
		this->read_registers();
		if ((this->registers[registers::STATUSRSSI] & (1 << radio_fm1::BIT_STC)) != 0)
			break; //Tuning complete!
	}

	this->read_registers();
	this->registers[registers::CHANNEL] &= 0x7FFF; //clear the tune after a tune has completed
	this->update_registers();

	//Wait for the si4703 to clear the STC as well
	while (true)
	{
		this->read_registers();
		if ((this->registers[registers::STATUSRSSI] & (1 << radio_fm1::BIT_STC)) == 0)
			break; //Tuning complete!
	}
}

double radio_fm1::get_channel()
{
	this->read_registers();

	int chan = this->registers[registers::READCHAN] & 0x03FF;

	return (chan * 2.0 + 875.0) / 10.0;
}

double radio_fm1::seek(seek_direction direction)
{
	this->read_registers();

	//Set seek mode wrap bit
	this->registers[registers::POWERCFG] &= 0xFBFF;


	if (direction == seek_directions::BACKWARD)
		this->registers[registers::POWERCFG] &= 0xFDFF; //seek down is the default upon reset
	else
		this->registers[registers::POWERCFG] |= 1 << radio_fm1::BIT_SEEKUP; //Set the bit to seek up


	this->registers[registers::POWERCFG] |= (1 << radio_fm1::BIT_SEEK); //Start seek
	this->update_registers();

	//Poll to see if STC is set
	while (true)
	{
		this->read_registers();
		if ((this->registers[registers::STATUSRSSI] & (1 << radio_fm1::BIT_STC)) != 0)
			break;
	}

	this->read_registers();
	int val = this->registers[registers::STATUSRSSI] & (1 << radio_fm1::BIT_SFBL);
	this->registers[registers::POWERCFG] &= 0xFEFF; //clear the seek bit after seek has completed
	this->update_registers();

	//Wait for the si4703 to clear the STC as well
	while (true)
	{
		this->read_registers();
		if ((this->registers[registers::STATUSRSSI] & (1 << radio_fm1::BIT_STC)) == 0)
			break;
	}

	if (val > 0) //The bit was set indicating we hit a band limit or failed to find a station
		return radio_fm1::INVALID_CHANNEL;

	return this->get_channel();
}
