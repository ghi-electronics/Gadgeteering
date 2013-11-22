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

#include "Types.h"

using namespace gadgeteering;

spi_configuration::spi_configuration()
{

}


spi_configuration::spi_configuration(bool cs_active_state, unsigned int cs_setup_time, unsigned int cs_hold_time, bool clock_idle_state, bool clock_edge, unsigned int clock_rate, bool uses_chip_select)
{
	this->cs_active_state = cs_active_state;
	this->cs_setup_time = cs_setup_time;
	this->cs_hold_time = cs_hold_time;
	this->clock_idle_state = clock_idle_state;
	this->clock_edge = clock_edge;
	this->clock_rate = clock_rate;
	this->uses_chip_select = uses_chip_select;
}

serial_configuration::serial_configuration(unsigned int baud_rate, serial_configuration::parity parity, serial_configuration::stop_bit stop_bits, unsigned char data_bits)
{
	this->baud_rate = baud_rate;
	this->data_parity = parity;
	this->stop_bits = stop_bits;
	this->data_bits = data_bits;
}

color::color()
{
	this->red = 0x00;
	this->green = 0x00;
	this->blue = 0x00;
}

color::color(unsigned char r, unsigned char g, unsigned char b)
{
	this->red = r;
	this->green = g;
	this->blue = b;
}

const color color::RED(0xFF, 0x00, 0x00);
const color color::GREEN(0x00, 0xFF, 0x00);
const color color::BLUE(0x00, 0x00, 0xFF);
const color color::WHITE(0xFF, 0xFF, 0xFF);
const color color::BLACK(0x00, 0x00, 0x00);
