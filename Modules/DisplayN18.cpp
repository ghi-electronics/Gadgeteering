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

#include "DisplayN18.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

display_n18::display_n18(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::S)), reset_pin(this->sock, 3), backlight_pin(this->sock, 4, true), rs_pin(this->sock, 5), spi(this->sock.spi, spi_configuration(false, 0, 0, false, true, 8000), this->sock, 6)
{
	this->initialize();
}

void display_n18::write_command(unsigned char command)
{
	this->rs_pin.write(false);
	this->spi.write(&command, 1, true);
}

void display_n18::write_data(unsigned char data)
{
	this->write_data(&data, 1);
}

void display_n18::write_data(const unsigned char* data, unsigned int length)
{
	this->rs_pin.write(true);
	this->spi.write(data, length, true);
}

void display_n18::reset()
{
	this->reset_pin.write(false);
	system::sleep(300);
	this->reset_pin.write(true);
	system::sleep(500);
}

void display_n18::initialize()
{
	this->reset();

	this->write_command(0x11);
	system::sleep(120);

	//ST7735R Frame Rate
	this->write_command(0xB1);
	this->write_data(0x01); this->write_data(0x2C); this->write_data(0x2D);
	this->write_command(0xB2);
	this->write_data(0x01); this->write_data(0x2C); this->write_data(0x2D);
	this->write_command(0xB3);
	this->write_data(0x01); this->write_data(0x2C); this->write_data(0x2D);
	this->write_data(0x01); this->write_data(0x2C); this->write_data(0x2D);

	this->write_command(0xB4); //Column inversion
	this->write_data(0x07);

	//ST7735R Power Sequence
	this->write_command(0xC0);
	this->write_data(0xA2); this->write_data(0x02); this->write_data(0x84);
	this->write_command(0xC1); this->write_data(0xC5);
	this->write_command(0xC2);
	this->write_data(0x0A); this->write_data(0x00);
	this->write_command(0xC3);
	this->write_data(0x8A); this->write_data(0x2A);
	this->write_command(0xC4);
	this->write_data(0x8A); this->write_data(0xEE);

	this->write_command(0xC5); //VCOM
	this->write_data(0x0E);

	this->write_command(0x36); //MX, MY, RGB mode
	this->write_data(0xC8);

	//ST7735R Gamma Sequence
	this->write_command(0xe0);
	this->write_data(0x0f); this->write_data(0x1a);
	this->write_data(0x0f); this->write_data(0x18);
	this->write_data(0x2f); this->write_data(0x28);
	this->write_data(0x20); this->write_data(0x22);
	this->write_data(0x1f); this->write_data(0x1b);
	this->write_data(0x23); this->write_data(0x37); this->write_data(0x00);
	this->write_data(0x07);
	this->write_data(0x02); this->write_data(0x10);

	this->write_command(0xe1);
	this->write_data(0x0f); this->write_data(0x1b);
	this->write_data(0x0f); this->write_data(0x17);
	this->write_data(0x33); this->write_data(0x2c);
	this->write_data(0x29); this->write_data(0x2e);
	this->write_data(0x30); this->write_data(0x30);
	this->write_data(0x39); this->write_data(0x3f);
	this->write_data(0x00); this->write_data(0x07);
	this->write_data(0x03); this->write_data(0x10);

	this->write_command(0x2a);
	this->write_data(0x00); this->write_data(0x00);
	this->write_data(0x00); this->write_data(0x7f);
	this->write_command(0x2b);
	this->write_data(0x00); this->write_data(0x00);
	this->write_data(0x00); this->write_data(0x9f);

	this->write_command(0xF0); //Enable test command
	this->write_data(0x01);
	this->write_command(0xF6); //Disable ram power save mode
	this->write_data(0x00);

	this->write_command(0x3A); //65k mode
	this->write_data(0x05);

	this->write_command(0x29);//Display on

	this->clear();
}

void display_n18::setup_draw(int x, int y, int width, int height)
{
	if (x > display_n18::WIDTH || y > display_n18::HEIGHT)
		return;

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;

	if (x + width > display_n18::WIDTH)
		width = display_n18::WIDTH - x;
	if (y + height > display_n18::HEIGHT)
		height = display_n18::HEIGHT - y;

	this->set_clipping_area(x, y, width - 1, height - 1);
	this->write_command(0x2C);
	this->rs_pin.write(true);
}

void display_n18::send_draw_data(const unsigned char* data, unsigned int length, bool is_end)
{
	this->spi.write(data, length, is_end);
}

void display_n18::send_draw_data(const unsigned short* data, unsigned int length, bool is_end)
{
	this->send_draw_data(reinterpret_cast<const unsigned char*>(data), length, is_end);
}

void display_n18::set_clipping_area(unsigned char x, unsigned char y, unsigned char width, unsigned char height)
{
	unsigned char data[4] = { 0x00, 0x00, 0x00, 0x00 };

	data[1] = x;
	data[3] = x + width;
	this->write_command(0x2A);
	this->write_data(data, 4);

	data[1] = y;
	data[3] = y + height;
	this->write_command(0x2B);
	this->write_data(data, 4);
}

unsigned short display_n18::rgb_to_short(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned short red = r;
	unsigned short green = g;
	unsigned short blue = b;

	red /= 8;
	green /= 4;
	blue /= 8;

	red &= 0x1F;
	green &= 0x3F;
	blue &= 0x1F;

	red <<= 3;
	blue <<= 8;
	green = ((green & 0x7) << 13) + ((green & 0x38) >> 3);

	return red | green | blue;
}

void display_n18::clear(unsigned short color)
{
	this->fill_rect(0, 0, display_n18::WIDTH, display_n18::HEIGHT, color);
}

void display_n18::draw(const unsigned short* data, int x, int y, int width, int height)
{
	this->draw(reinterpret_cast<const unsigned char*>(data), x, y, width, height);
}

void display_n18::draw(const unsigned char* data, int x, int y, int width, int height)
{
	if (x > display_n18::WIDTH || y > display_n18::HEIGHT)
		return;

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;

	if (x + width > display_n18::WIDTH)
		width = display_n18::WIDTH - x;
	if (y + height > display_n18::HEIGHT)
		height = display_n18::HEIGHT - y;

	this->set_clipping_area(x, y, width - 1, height - 1);
	this->write_command(0x2C);
	this->write_data(data, width * height * 2);
}

void display_n18::set_pixel(int x, int y, unsigned short color)
{
	this->draw(reinterpret_cast<unsigned char*>(&color), x, y, 1, 1);
}

void display_n18::fill_rect(int x, int y, int width, int height, unsigned short color)
{
	this->set_clipping_area((unsigned char)x, (unsigned char)y, (unsigned char)width - 1, (unsigned char)height);
	this->write_command(0x2C);

	unsigned short* buffer = new unsigned short[width * display_n18::STEP];
	for (int j = 0; j < width * display_n18::STEP; j++)
		buffer[j] = color;

	this->rs_pin.write(true);

	int i;
	for (i = display_n18::STEP; i <= height; i += display_n18::STEP)
		this->write_data(reinterpret_cast<unsigned char*>(buffer), width * display_n18::STEP * 2);

	i -= display_n18::STEP;
	if (height - i > 0)
		this->write_data(reinterpret_cast<unsigned char*>(buffer), width * (height - i) * 2);

	delete[] buffer;
}

void display_n18::draw_rect(int x, int y, int width, int height, unsigned short color)
{
	this->draw_line(x, y, x + width, y, color);
	this->draw_line(x, y + height, x + width, y + height, color);
	this->draw_line(x, y, x, y + height, color);
	this->draw_line(x + width, y, x + width, y + height, color);
}

void display_n18::fill_circle(int x, int y, int radius, unsigned short color)
{
	int f = 1 - radius;
	int dd_f_x = 1;
	int dd_f_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;

	for (int i = y - radius; i <= y + radius; i++)
		this->set_pixel(x, i, color);

	while (x1 < y1)
	{
		if (f >= 0)
		{
			y1--;
			dd_f_y += 2;
			f += dd_f_y;
		}

		x1++;
		dd_f_x += 2;
		f += dd_f_x;

		for (int i = y - y1; i <= y + y1; i++)
		{
			this->set_pixel(x + x1, i, color);
			this->set_pixel(x - x1, i, color);
		}

		for (int i = y - x1; i <= y + x1; i++)
		{
			this->set_pixel(x + y1, i, color);
			this->set_pixel(x - y1, i, color);
		}
	}
}

void display_n18::draw_circle(int x, int y, int radius, unsigned short color)
{
	int f = 1 - radius;
	int dd_f_x = 1;
	int dd_f_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;

	this->set_pixel(x, y + radius, color);
	this->set_pixel(x, y - radius, color);
	this->set_pixel(x + radius, y, color);
	this->set_pixel(x - radius, y, color);

	while (x1 < y1)
	{
		if (f >= 0)
		{
			y1--;
			dd_f_y += 2;
			f += dd_f_y;
		}

		x1++;
		dd_f_x += 2;
		f += dd_f_x;

		this->set_pixel(x + x1, y + y1, color);
		this->set_pixel(x - x1, y + y1, color);
		this->set_pixel(x + x1, y - y1, color);
		this->set_pixel(x - x1, y - y1, color);

		this->set_pixel(x + y1, y + x1, color);
		this->set_pixel(x - y1, y + x1, color);
		this->set_pixel(x + y1, y - x1, color);
		this->set_pixel(x - y1, y - x1, color);
	}
}

void display_n18::draw_line(int x0, int y0, int x1, int y1, unsigned short color)
{
	if (x0 == x1)
	{
		if (y1 < y0)
		{
			int temp = y0;
			y0 = y1;
			y1 = temp;
		}

		this->set_clipping_area((unsigned char)x0, (unsigned char)y0, 1, (unsigned char)y1 - y0 - 1);
		this->write_command(0x2C);

		unsigned short data[display_n18::STEP_Y];
		for (int i = 0; i < display_n18::STEP_Y; i++)
			data[i] = color;

		for (unsigned char thisY = y0; thisY < y1; thisY += display_n18::STEP_Y)
			this->write_data(reinterpret_cast<unsigned char*>(data), (y1 - thisY >= display_n18::STEP_Y ? display_n18::STEP_Y : y1 - thisY) * 2);

		return;
	}

	if (y0 == y1)
	{
		if (x1 < x0)
		{
			int temp = x0;
			x0 = x1;
			x1 = temp;
		}

		this->set_clipping_area((unsigned char)x0, (unsigned char)x0, (unsigned char)x1 - x0 - 1, 1);
		this->write_command(0x2C);

		unsigned short data[display_n18::STEP_X];
		for (int i = 0; i < display_n18::STEP_X; i++)
			data[i] = color;

		for (unsigned char thisX = x0; thisX < x1; thisX += display_n18::STEP_X)
			this->write_data(reinterpret_cast<unsigned char*>(data), (x1 - thisX >= display_n18::STEP_X ? display_n18::STEP_X : x1 - thisX) * 2);

		return;
	}

	int t;
	bool steep = ((y1 - y0) < 0 ? -(y1 - y0) : (y1 - y0)) > ((x1 - x0) < 0 ? -(x1 - x0) : (x1 - x0));

	if (steep)
	{
		t = x0;
		x0 = y0;
		y0 = t;
		t = x1;
		x1 = y1;
		y1 = t;
	}

	if (x0 > x1)
	{
		t = x0;
		x0 = x1;
		x1 = t;

		t = y0;
		y0 = y1;
		y1 = t;
	}

	int dx, dy;
	dx = x1 - x0;
	dy = (y1 - y0) < 0 ? -(y1 - y0) : (y1 - y0);

	int err = (dx / 2);
	int ystep;

	if (y0 < y1)
		ystep = 1;
	else
		ystep = -1;

	for (; x0 < x1; x0++)
	{
		if (steep)
			this->set_pixel(y0, x0, color);
		else
			this->set_pixel(x0, y0, color);

		err -= dy;

		if (err < 0)
		{
			y0 += (char)ystep;
			err += dx;
		}
	}
}


#ifdef Arduino_h
#include <avr/pgmspace.h>

PROGMEM prog_char characters[95 * 5] = {
#else
unsigned char characters[95][5] = {
#endif
	0x00, 0x00, 0x00, 0x00, 0x00, /* Space	0x20 */
	0x00, 0x00, 0x4f, 0x00, 0x00, /* ! */
	0x00, 0x07, 0x00, 0x07, 0x00, /* " */
	0x14, 0x7f, 0x14, 0x7f, 0x14, /* # */
	0x24, 0x2a, 0x7f, 0x2a, 0x12, /* $ */
	0x23, 0x13, 0x08, 0x64, 0x62, /* % */
	0x36, 0x49, 0x55, 0x22, 0x20, /* & */
	0x00, 0x05, 0x03, 0x00, 0x00, /* ' */
	0x00, 0x1c, 0x22, 0x41, 0x00, /* ( */
	0x00, 0x41, 0x22, 0x1c, 0x00, /* ) */
	0x14, 0x08, 0x3e, 0x08, 0x14, /* // */
	0x08, 0x08, 0x3e, 0x08, 0x08, /* + */
	0x50, 0x30, 0x00, 0x00, 0x00, /* , */
	0x08, 0x08, 0x08, 0x08, 0x08, /* - */
	0x00, 0x60, 0x60, 0x00, 0x00, /* . */
	0x20, 0x10, 0x08, 0x04, 0x02, /* / */
	0x3e, 0x51, 0x49, 0x45, 0x3e, /* 0		0x30 */
	0x00, 0x42, 0x7f, 0x40, 0x00, /* 1 */
	0x42, 0x61, 0x51, 0x49, 0x46, /* 2 */
	0x21, 0x41, 0x45, 0x4b, 0x31, /* 3 */
	0x18, 0x14, 0x12, 0x7f, 0x10, /* 4 */
	0x27, 0x45, 0x45, 0x45, 0x39, /* 5 */
	0x3c, 0x4a, 0x49, 0x49, 0x30, /* 6 */
	0x01, 0x71, 0x09, 0x05, 0x03, /* 7 */
	0x36, 0x49, 0x49, 0x49, 0x36, /* 8 */
	0x06, 0x49, 0x49, 0x29, 0x1e, /* 9 */
	0x00, 0x36, 0x36, 0x00, 0x00, /* : */
	0x00, 0x56, 0x36, 0x00, 0x00, /* ; */
	0x08, 0x14, 0x22, 0x41, 0x00, /* < */
	0x14, 0x14, 0x14, 0x14, 0x14, /* = */
	0x00, 0x41, 0x22, 0x14, 0x08, /* > */
	0x02, 0x01, 0x51, 0x09, 0x06, /* ? */
	0x3e, 0x41, 0x5d, 0x55, 0x1e, /* @		0x40 */
	0x7e, 0x11, 0x11, 0x11, 0x7e, /* A */
	0x7f, 0x49, 0x49, 0x49, 0x36, /* B */
	0x3e, 0x41, 0x41, 0x41, 0x22, /* C */
	0x7f, 0x41, 0x41, 0x22, 0x1c, /* D */
	0x7f, 0x49, 0x49, 0x49, 0x41, /* E */
	0x7f, 0x09, 0x09, 0x09, 0x01, /* F */
	0x3e, 0x41, 0x49, 0x49, 0x7a, /* G */
	0x7f, 0x08, 0x08, 0x08, 0x7f, /* H */
	0x00, 0x41, 0x7f, 0x41, 0x00, /* I */
	0x20, 0x40, 0x41, 0x3f, 0x01, /* J */
	0x7f, 0x08, 0x14, 0x22, 0x41, /* K */
	0x7f, 0x40, 0x40, 0x40, 0x40, /* L */
	0x7f, 0x02, 0x0c, 0x02, 0x7f, /* M */
	0x7f, 0x04, 0x08, 0x10, 0x7f, /* N */
	0x3e, 0x41, 0x41, 0x41, 0x3e, /* O */
	0x7f, 0x09, 0x09, 0x09, 0x06, /* P		0x50 */
	0x3e, 0x41, 0x51, 0x21, 0x5e, /* Q */
	0x7f, 0x09, 0x19, 0x29, 0x46, /* R */
	0x26, 0x49, 0x49, 0x49, 0x32, /* S */
	0x01, 0x01, 0x7f, 0x01, 0x01, /* T */
	0x3f, 0x40, 0x40, 0x40, 0x3f, /* U */
	0x1f, 0x20, 0x40, 0x20, 0x1f, /* V */
	0x3f, 0x40, 0x38, 0x40, 0x3f, /* W */
	0x63, 0x14, 0x08, 0x14, 0x63, /* X */
	0x07, 0x08, 0x70, 0x08, 0x07, /* Y */
	0x61, 0x51, 0x49, 0x45, 0x43, /* Z */
	0x00, 0x7f, 0x41, 0x41, 0x00, /* [ */
	0x02, 0x04, 0x08, 0x10, 0x20, /* \ */
	0x00, 0x41, 0x41, 0x7f, 0x00, /* ] */
	0x04, 0x02, 0x01, 0x02, 0x04, /* ^ */
	0x40, 0x40, 0x40, 0x40, 0x40, /* _ */
	0x00, 0x00, 0x03, 0x05, 0x00, /* `		0x60 */
	0x20, 0x54, 0x54, 0x54, 0x78, /* a */
	0x7F, 0x44, 0x44, 0x44, 0x38, /* b */
	0x38, 0x44, 0x44, 0x44, 0x44, /* c */
	0x38, 0x44, 0x44, 0x44, 0x7f, /* d */
	0x38, 0x54, 0x54, 0x54, 0x18, /* e */
	0x04, 0x04, 0x7e, 0x05, 0x05, /* f */
	0x08, 0x54, 0x54, 0x54, 0x3c, /* g */
	0x7f, 0x08, 0x04, 0x04, 0x78, /* h */
	0x00, 0x44, 0x7d, 0x40, 0x00, /* i */
	0x20, 0x40, 0x44, 0x3d, 0x00, /* j */
	0x7f, 0x10, 0x28, 0x44, 0x00, /* k */
	0x00, 0x41, 0x7f, 0x40, 0x00, /* l */
	0x7c, 0x04, 0x7c, 0x04, 0x78, /* m */
	0x7c, 0x08, 0x04, 0x04, 0x78, /* n */
	0x38, 0x44, 0x44, 0x44, 0x38, /* o */
	0x7c, 0x14, 0x14, 0x14, 0x08, /* p		0x70 */
	0x08, 0x14, 0x14, 0x14, 0x7c, /* q */
	0x7c, 0x08, 0x04, 0x04, 0x00, /* r */
	0x48, 0x54, 0x54, 0x54, 0x24, /* s */
	0x04, 0x04, 0x3f, 0x44, 0x44, /* t */
	0x3c, 0x40, 0x40, 0x20, 0x7c, /* u */
	0x1c, 0x20, 0x40, 0x20, 0x1c, /* v */
	0x3c, 0x40, 0x30, 0x40, 0x3c, /* w */
	0x44, 0x28, 0x10, 0x28, 0x44, /* x */
	0x0c, 0x50, 0x50, 0x50, 0x3c, /* y */
	0x44, 0x64, 0x54, 0x4c, 0x44, /* z */
	0x08, 0x36, 0x41, 0x41, 0x00, /* { */
	0x00, 0x00, 0x77, 0x00, 0x00, /* | */
	0x00, 0x41, 0x41, 0x36, 0x08, /* } */
	0x08, 0x08, 0x2a, 0x1c, 0x08  /* ~ */
};

void display_n18::draw_character(int x, int y, const char character, unsigned short fore_color, unsigned short back_color, unsigned char font_size)
{
	if (character > 126 || character < 32)
		return;

	unsigned short* horizontal = new unsigned short[display_n18::CHAR_HEIGHT * font_size];
	for (int i = 0; i < display_n18::CHAR_WIDTH; i++)
	{
		for (int j = 0; j < display_n18::CHAR_HEIGHT; j++)
		{
			for (int k = 0; k < font_size; k++)
#ifdef Arduino_h
				horizontal[j * font_size + k] = pgm_read_byte_near(characters + (character - 32) * 5 + i) & (1 << j) ? fore_color : back_color;
#else
				horizontal[j * font_size + k] = characters[character][i] & (1 << j) ? fore_color : back_color;
#endif
		}

		for (int k = 0; k < font_size; k++)
			this->draw(horizontal, x + i * font_size + k, y, 1, display_n18::CHAR_HEIGHT * font_size);
	}

	for (int i = 0; i < display_n18::CHAR_HEIGHT; i++)
	for (int k = 0; k < font_size; k++)
		horizontal[i * font_size + k] = back_color;

	for (int k = 0; k < font_size; k++)
		this->draw(horizontal, x + display_n18::CHAR_WIDTH * font_size + k, y, 1, display_n18::CHAR_HEIGHT * font_size);

	delete[] horizontal;
}

void display_n18::draw_strong(int x, int y, const char* str, unsigned short fore_color, unsigned short back_color, unsigned char font_size)
{
	if (*str == '\0')
		return;

	do
	{
		this->draw_character(x, y, *str, fore_color, back_color, font_size);
		x += (display_n18::CHAR_WIDTH + display_n18::CHAR_SPACING) * font_size;
	} while (*(++str) != '\0');
}
