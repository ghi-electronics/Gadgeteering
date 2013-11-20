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

#include "MultiColorLED.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

multicolor_led::multicolor_led(unsigned char socket_number) : daisy_link::module(socket_number, multicolor_led::GHI_DAISYLINK_MANUFACTURER, multicolor_led::GHI_DAISYLINK_TYPE_MULTICOLORLED, multicolor_led::GHI_DAISYLINK_VERSION_MULTICOLORLED, multicolor_led::GHI_DAISYLINK_VERSION_MULTICOLORLED)
{
	this->green_blue_swapped = false;

	this->turn_off();
}

bool multicolor_led::swap_green_blue()
{
	this->green_blue_swapped = true;

	return this->green_blue_swapped;
}

void multicolor_led::turn_blue()
{
	this->send_command(color::BLUE, modes::Constant);
}

void multicolor_led::turn_red()
{
	this->send_command(color::RED, modes::Constant);
}

void multicolor_led::turn_green()
{
	this->send_command(color::GREEN, modes::Constant);
}

void multicolor_led::turn_off()
{
	this->send_command(color::BLACK, modes::Constant);
}

void multicolor_led::turn_white()
{
	this->send_command(color::WHITE, modes::Constant);
}

void multicolor_led::turn_color(color c)
{
	this->send_command(c, modes::Constant);
}

void multicolor_led::set_red_intensity(unsigned char intensity)
{
	color current_color = this->get_current_color();
	current_color.red = intensity;
	this->send_command(current_color);
}

void multicolor_led::set_red_intensity(int intensity)
{
	if (intensity < 0) intensity = 0;
	if (intensity > 255) intensity = 255;
	this->set_red_intensity(static_cast<unsigned char>(intensity));
}

void multicolor_led::set_green_intensity(unsigned char intensity)
{
	color current_color = this->get_current_color();
	current_color.green = intensity;
	this->send_command(current_color);
}

void multicolor_led::set_green_intensity(int intensity)
{
	if (intensity < 0) intensity = 0;
	if (intensity > 255) intensity = 255;
	set_green_intensity(static_cast<unsigned char>(intensity));
}

void multicolor_led::set_blue_intensity(unsigned char intensity)
{
	color current_color = this->get_current_color();
	current_color.blue = intensity;
	this->send_command(current_color);
}

void multicolor_led::set_blue_intensity(int intensity)
{
	if (intensity < 0) intensity = 0;
	if (intensity > 255) intensity = 255;
	set_blue_intensity(static_cast<unsigned char>(intensity));
}

void multicolor_led::add_red()
{
	color current_color = this->get_current_color();
	current_color.red = 255;
	this->send_command(current_color);
}

void multicolor_led::remove_red()
{
	color current_color = this->get_current_color();
	current_color.red = 0;
	this->send_command(current_color);
}

void multicolor_led::add_green()
{
	color current_color = this->get_current_color();
	current_color.green = 255;
	this->send_command(current_color);
}

void multicolor_led::remove_green()
{
	color current_color = this->get_current_color();
	current_color.green = 0;
	this->send_command(current_color);
}

void multicolor_led::add_blue()
{
	color current_color = this->get_current_color();
	current_color.blue = 255;
	this->send_command(current_color);
}

void multicolor_led::remove_blue()
{
	color current_color = this->get_current_color();
	current_color.blue = 0;
	this->send_command(current_color);
}

color multicolor_led::get_current_color()
{
	// NOTE: THE COLOR RETURNED IS THE COLOR YOU WANT IT TO BE. THIS IS NOT AFFECTED BY THE COLOR SWAP.
	unsigned char c1 = this->read_register(static_cast<unsigned char>(daisy_link::module::REGISTER_OFFSET + registers::Color1));
	unsigned char c2 = this->read_register(static_cast<unsigned char>(daisy_link::module::REGISTER_OFFSET + registers::Color1 + 1));
	unsigned char c3 = this->read_register(static_cast<unsigned char>(daisy_link::module::REGISTER_OFFSET + registers::Color1 + 2));

	return green_blue_swapped ? color(c1, c3, c2) : color(c1, c2, c3);
}

void multicolor_led::blink_once(color c)
{
	this->send_command(c, 1000, color::BLACK, 0, modes::blink_onceInt);
}

void multicolor_led::blink_once(color c, unsigned int blink_time)
{
	this->send_command(c, blink_time, color::BLACK, 0, modes::blink_onceInt);
}

void multicolor_led::blink_once(color blink_color, unsigned int blink_time, color end_color)
{
	this->send_command(blink_color, blink_time, end_color, 0, modes::blink_onceInt);
}

void multicolor_led::blink_repeatedly(color c)
{
	this->send_command(c, 1000, color::BLACK, 1000, modes::blink_repeatedly);
}

void multicolor_led::blink_repeatedly(color color_1, unsigned int blink_time_1, color color_2, unsigned int blink_time_2)
{
	this->send_command(color_1, blink_time_1, color_2, blink_time_2, modes::blink_repeatedly);
}

void multicolor_led::fade_once(color c)
{
	this->send_command(c, 1000, color::BLACK, 0, modes::fade_onceInt);
}

void multicolor_led::fade_once(color c, unsigned int fadeTime)
{
	this->send_command(c, fadeTime, color::BLACK, 0, modes::fade_onceInt);
}

void multicolor_led::fade_once(color from_color, unsigned int fadeTime, color to_color)
{
	this->send_command(from_color, fadeTime, to_color, 0, modes::fade_onceInt);
}

void multicolor_led::fade_repeatedly(color c)
{
	this->send_command(c, 1000, color::BLACK, 1000, modes::fade_repeatedly);
}

void multicolor_led::fade_repeatedly(color color_1, unsigned int fade_time_1, color color_2, unsigned int fade_time_2)
{
	this->send_command(color_1, fade_time_1, color_2, fade_time_2, modes::fade_repeatedly);
}

void multicolor_led::send_command(color color_1, unsigned int blink_time_1, color color_2, unsigned int blink_time_2, mode mode)
{
	//10000 ticks in a millisecond, from the .net version
	long time1 = blink_time_1 * 10000 / 1000;
	long time2 = blink_time_2 * 10000 / 1000;

	// send the parameters with mode off to avoid side effects of previous mode
	if (this->green_blue_swapped)
	{
		unsigned char data[17] = { static_cast<unsigned char>(daisy_link::module::REGISTER_OFFSET + registers::Configuration), static_cast<unsigned char>(modes::Off), 0x00,
			color_1.red, color_1.blue, color_1.green,
			color_2.red, color_2.blue, color_2.green,
			static_cast<unsigned char>(time1 >> 0), static_cast<unsigned char>(time1 >> 8), static_cast<unsigned char>(time1 >> 16), static_cast<unsigned char>(time1 >> 24),
			static_cast<unsigned char>(time2 >> 0), static_cast<unsigned char>(time2 >> 8), static_cast<unsigned char>(time2 >> 16), static_cast<unsigned char>(time2 >> 24) };

		this->write(data, 17);
	}
	else
	{
		unsigned char data[17] = { static_cast<unsigned char>(daisy_link::module::REGISTER_OFFSET + registers::Configuration), static_cast<unsigned char>(modes::Off), 0x00,
			color_1.red, color_1.green, color_1.blue,
			color_2.red, color_2.green, color_2.blue,
			static_cast<unsigned char>(time1 >> 0), static_cast<unsigned char>(time1 >> 8), static_cast<unsigned char>(time1 >> 16), static_cast<unsigned char>(time1 >> 24),
			static_cast<unsigned char>(time2 >> 0), static_cast<unsigned char>(time2 >> 8), static_cast<unsigned char>(time2 >> 16), static_cast<unsigned char>(time2 >> 24) };
		this->write(data, 17);
	}
	// now activate the correct mode
	unsigned char data[3] = { static_cast<unsigned char>(daisy_link::module::REGISTER_OFFSET + registers::Configuration), static_cast<unsigned char>(mode), 0x1 };
	this->write(data, 3);
}

void multicolor_led::send_command(color c, mode mode)
{
	// send the parameters with mode off to avoid side effects of previous mode
	if (this->green_blue_swapped)
	{
		unsigned char data[6] = { static_cast<unsigned char>(daisy_link::module::REGISTER_OFFSET + registers::Configuration), static_cast<unsigned char>(modes::Off), 0x0, c.red, c.blue, c.green };
		this->write(data, 6);
	}
	else
	{
		unsigned char data[6] = { static_cast<unsigned char>(daisy_link::module::REGISTER_OFFSET + registers::Configuration), static_cast<unsigned char>(modes::Off), 0x0, c.red, c.green, c.blue };
		this->write(data, 6);
	}
	// now activate the correct mode
	unsigned char data[3] = { static_cast<unsigned char>(daisy_link::module::REGISTER_OFFSET + registers::Configuration), static_cast<unsigned char>(mode), 0x1 };
	this->write(data, 3);
}

void multicolor_led::send_command(color c)
{
	mode current_mode = static_cast<mode>(this->read_register(static_cast<unsigned char>(daisy_link::module::REGISTER_OFFSET + registers::Configuration)));
	if (current_mode == modes::Off)
	{
		if (this->green_blue_swapped)
		{
			unsigned char data[6] = { static_cast<unsigned char>(daisy_link::module::REGISTER_OFFSET + registers::Configuration), static_cast<unsigned char>(modes::Constant), 0x1, c.red,
				c.blue, c.green };
			this->write(data, 6);
		}
		else
		{
			unsigned char data[6] = { static_cast<unsigned char>(daisy_link::module::REGISTER_OFFSET + registers::Configuration), static_cast<unsigned char>(modes::Constant), 0x1, c.red,
				c.green, c.blue };
			this->write(data, 6);
		}
	}
	else
	{
		if (green_blue_swapped)
		{
			unsigned char data[4] = { static_cast<unsigned char>(daisy_link::module::REGISTER_OFFSET + registers::Color1), c.red, c.blue, c.green };
			this->write(data, 4);
		}
		else
		{
			unsigned char data[4] = { static_cast<unsigned char>(daisy_link::module::REGISTER_OFFSET + registers::Color1), c.red, c.green, c.blue };
			this->write(data, 4);
		}
	}
}
