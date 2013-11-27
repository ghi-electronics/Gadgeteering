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

#include "FEZtive.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;

feztive::feztive(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::S)), spi(this->sock.spi, spi_configuration(true, 0, 0, false, true, 1000), this->sock, 6)
{
	this->leds = NULL;
	this->zeros = NULL;
}

feztive::~feztive() 
{
	if (this->leds)
		delete[] this->leds;
	if (this->zeros)
		delete[] this->zeros;
}

void feztive::initialize(int num_leds, unsigned int spi_clock)
{
	this->spi.config.clock_rate = spi_clock;

	this->led_len = num_leds;
	this->leds = new color[num_leds];

	for (int i = 0; i < num_leds; i++)
	{
		this->leds[i] = color(0, 0, 0);
	}

	this->zero_len = 3 * ((num_leds + 63) / 64);
	this->zeros = new unsigned char[this->zero_len];

	for (unsigned int i = 0; i < this->zero_len; i++)
		this->zeros[i] = 0x00;
}

void feztive::set_all(color c)
{
	unsigned char colorarr[3];

	for (int i = 0; i < 3; i++)
		colorarr[i] = 0;

	//clear();

	this->spi.write(this->zeros, 1);

	for (unsigned int i = 0; i < this->led_len; i += 2)
	{
		this->leds[i] = c;
		this->leds[i + 1] = c;

		this->get_color_for_render(this->leds[i], colorarr[2], colorarr[1], colorarr[0]);
		this->spi.write(colorarr, 3);
		this->get_color_for_render(this->leds[i + 1], colorarr[2], colorarr[1], colorarr[0]);
		this->spi.write(colorarr, 3);
	}

	this->spi.write(this->zeros, 1);
}

void feztive::set_all(color *arr)
{
	unsigned char c[3];

	this->spi.write(this->zeros, 1);

	for (unsigned int i = 0; i < this->led_len; i += 2)
	{
		this->set_led(arr[i], i);
		this->set_led(arr[i + 1], i + 1);

		this->get_color_for_render(this->leds[i], c[2], c[1], c[0]);
		this->spi.write(c, 1);
		this->get_color_for_render(this->leds[i + 1], c[2], c[1], c[0]);
		this->spi.write(c, 1);
	}

	this->spi.write(this->zeros, 1);
}

void feztive::set_led(color c, int num_led, bool redraw)
{
	this->leds[num_led] = c;

	if (redraw)
		this->redraw();
}

color *feztive::get_current_colors()
{
	return this->leds;
}

void feztive::clear()
{
	this->set_all(color(0, 0, 0));
}

void feztive::redraw()
{
	unsigned char c[3];

	this->spi.write(this->zeros, 1);

	for (unsigned int i = 0; i < this->led_len; i += 2)
	{
		this->get_color_for_render(this->leds[i], c[2], c[1], c[0]);
		this->spi.write(c, 3);
		this->get_color_for_render(this->leds[i + 1], c[2], c[1], c[0]);
		this->spi.write(c, 3);
	}

	this->spi.write(this->zeros, 1);
}

void feztive::get_color_for_render(color c, unsigned char& g, unsigned char& r, unsigned char& b)
{
	g = (0x80 | c.green);
	r = (0x80 | c.red);
	b = (0x80 | c.blue);
}

color feztive::generate_random_color()
{
	color c;

	system::random_seed(static_cast<int>(system::time_elapsed()));

	c.red = (system::random_number(0, 127) / system::random_number(1, 4));
	c.green = (system::random_number(0, 127) / system::random_number(1, 4));
	c.blue = (system::random_number(0, 127) / system::random_number(1, 4));

	return c;
}
