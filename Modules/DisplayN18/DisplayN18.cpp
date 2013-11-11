/*
Copyright 2013 Gadgeteering Electronics LLC

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

using namespace Gadgeteering;
using namespace Gadgeteering::Modules;
using namespace Gadgeteering::Interfaces;

DisplayN18::DisplayN18(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::S);

	this->resetPin = new DigitalOutput(socket, Socket::Pins::Three, false);
	this->backlightPin = new DigitalOutput(socket, Socket::Pins::Four, true);
	this->rsPin = new DigitalOutput(socket, Socket::Pins::Five, false);
	this->spi = socket->getSPIBus()->getSPIDevice(socket->pins[6], new SPIConfiguration(false, 0, 0, false, true, 8000));

	this->initialize();
}

DisplayN18::~DisplayN18() {
	delete this->resetPin;
	delete this->backlightPin;
	delete this->rsPin;
}

void DisplayN18::writeCommand(unsigned char command) {
	this->rsPin->write(false);
	this->spi->write(&command, 1, true);
}

void DisplayN18::writeData(unsigned char data) {
	this->writeData(&data, 1);
}

void DisplayN18::writeData(const unsigned char* data, unsigned int length) {
	this->rsPin->write(true);
	this->spi->write(data, length, true);
}

void DisplayN18::reset() {
	this->resetPin->write(false);
	System::Sleep(300);
	this->resetPin->write(true);
	System::Sleep(500);
}

void DisplayN18::initialize() {
	this->reset();

	this->writeCommand(0x11);
	System::Sleep(120);

	//ST7735R Frame Rate
	this->writeCommand(0xB1);
	this->writeData(0x01); this->writeData(0x2C); this->writeData(0x2D);
	this->writeCommand(0xB2);
	this->writeData(0x01); this->writeData(0x2C); this->writeData(0x2D);
	this->writeCommand(0xB3);
	this->writeData(0x01); this->writeData(0x2C); this->writeData(0x2D);
	this->writeData(0x01); this->writeData(0x2C); this->writeData(0x2D);

	this->writeCommand(0xB4); //Column inversion
	this->writeData(0x07);

	//ST7735R Power Sequence
	this->writeCommand(0xC0);
	this->writeData(0xA2); this->writeData(0x02); this->writeData(0x84);
	this->writeCommand(0xC1); this->writeData(0xC5);
	this->writeCommand(0xC2);
	this->writeData(0x0A); this->writeData(0x00);
	this->writeCommand(0xC3);
	this->writeData(0x8A); this->writeData(0x2A);
	this->writeCommand(0xC4);
	this->writeData(0x8A); this->writeData(0xEE);

	this->writeCommand(0xC5); //VCOM
	this->writeData(0x0E);

	this->writeCommand(0x36); //MX, MY, RGB mode
	this->writeData(0xC8);

	//ST7735R Gamma Sequence
	this->writeCommand(0xe0);
	this->writeData(0x0f); this->writeData(0x1a);
	this->writeData(0x0f); this->writeData(0x18);
	this->writeData(0x2f); this->writeData(0x28);
	this->writeData(0x20); this->writeData(0x22);
	this->writeData(0x1f); this->writeData(0x1b);
	this->writeData(0x23); this->writeData(0x37); this->writeData(0x00);
	this->writeData(0x07);
	this->writeData(0x02); this->writeData(0x10);

	this->writeCommand(0xe1);
	this->writeData(0x0f); this->writeData(0x1b);
	this->writeData(0x0f); this->writeData(0x17);
	this->writeData(0x33); this->writeData(0x2c);
	this->writeData(0x29); this->writeData(0x2e);
	this->writeData(0x30); this->writeData(0x30);
	this->writeData(0x39); this->writeData(0x3f);
	this->writeData(0x00); this->writeData(0x07);
	this->writeData(0x03); this->writeData(0x10);

	this->writeCommand(0x2a);
	this->writeData(0x00); this->writeData(0x00);
	this->writeData(0x00); this->writeData(0x7f);
	this->writeCommand(0x2b);
	this->writeData(0x00); this->writeData(0x00);
	this->writeData(0x00); this->writeData(0x9f);

	this->writeCommand(0xF0); //Enable test command
	this->writeData(0x01);
	this->writeCommand(0xF6); //Disable ram power save mode
	this->writeData(0x00);

	this->writeCommand(0x3A); //65k mode
	this->writeData(0x05);

	this->writeCommand(0x29);//Display on

	this->clear();
}

void DisplayN18::setupDraw(int x, int y, int width, int height) {
	if (x > DisplayN18::WIDTH || y > DisplayN18::HEIGHT)
		return;

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;

	if (x + width > DisplayN18::WIDTH)
		width = DisplayN18::WIDTH - x;
	if (y + height > DisplayN18::HEIGHT)
		height = DisplayN18::HEIGHT - y;

	this->setClippingArea(x, y, width - 1, height - 1);
	this->writeCommand(0x2C);
	this->rsPin->write(true);
}

void DisplayN18::sendDrawData(const unsigned char* data, unsigned int length, bool deselectChip) {
	this->spi->write(data, length, deselectChip);
}

void DisplayN18::sendDrawData(const unsigned short* data, unsigned int length, bool deselectChip) {
	this->sendDrawData(reinterpret_cast<const unsigned char*>(data), length, deselectChip);
}

void DisplayN18::setClippingArea(unsigned char x, unsigned char y, unsigned char width, unsigned char height) {
	unsigned char data[4] = { 0x00, 0x00, 0x00, 0x00 };

	data[1] = x;
	data[3] = x + width;
	this->writeCommand(0x2A);
	this->writeData(data, 4);

	data[1] = y;
	data[3] = y + height;
	this->writeCommand(0x2B);
	this->writeData(data, 4);
}

unsigned short DisplayN18::rgbToShort(unsigned char r, unsigned char g, unsigned char b) {
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

void DisplayN18::clear(unsigned short color) {
	this->fillRect(0, 0, DisplayN18::WIDTH, DisplayN18::HEIGHT, color);
}

void DisplayN18::draw(const unsigned short* data, int x, int y, int width, int height) {
	this->draw(reinterpret_cast<const unsigned char*>(data), x, y, width, height);
}

void DisplayN18::draw(const unsigned char* data, int x, int y, int width, int height) {
	if (x > DisplayN18::WIDTH || y > DisplayN18::HEIGHT)
		return;

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;

	if (x + width > DisplayN18::WIDTH)
		width = DisplayN18::WIDTH - x;
	if (y + height > DisplayN18::HEIGHT)
		height = DisplayN18::HEIGHT - y;

	this->setClippingArea(x, y, width - 1, height - 1);
	this->writeCommand(0x2C);
	this->writeData(data, width * height * 2);
}

void DisplayN18::setPixel(int x, int y, unsigned short color) {
	this->draw(reinterpret_cast<unsigned char*>(&color), x, y, 1, 1);
}

void DisplayN18::fillRect(int x, int y, int width, int height, unsigned short color) {
	this->setClippingArea((unsigned char)x, (unsigned char)y, (unsigned char)width - 1, (unsigned char)height);
	this->writeCommand(0x2C);

	unsigned short* buffer = new unsigned short[width * DisplayN18::STEP];
	for (int j = 0; j < width * DisplayN18::STEP; j++)
		buffer[j] = color;

	this->rsPin->write(true);

	int i;
	for (i = DisplayN18::STEP; i <= height; i += DisplayN18::STEP)
		this->writeData(reinterpret_cast<unsigned char*>(buffer), width * DisplayN18::STEP * 2);

	i -= DisplayN18::STEP;
	if (height - i > 0)
		this->writeData(reinterpret_cast<unsigned char*>(buffer), width * (height - i)* 2);

	delete[] buffer;
}

void DisplayN18::drawRect(int x, int y, int width, int height, unsigned short color) {
	this->drawLine(x, y, x + width, y, color);
	this->drawLine(x, y + height, x + width, y + height, color);
	this->drawLine(x, y, x, y + height, color);
	this->drawLine(x + width, y, x + width, y + height, color);
}

void DisplayN18::fillCircle(int x, int y, int radius, unsigned short color) {
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x1 = 0;
    int y1 = radius;

    for (int i = y - radius; i <= y + radius; i++)
        this->setPixel(x, i, color);

    while (x1 < y1) {
        if (f >= 0) {
            y1--;
            ddF_y += 2;
            f += ddF_y;
        }

        x1++;
        ddF_x += 2;
        f += ddF_x;

        for (int i = y - y1; i <= y + y1; i++) {
            this->setPixel(x + x1, i, color);
            this->setPixel(x - x1, i, color);
        }

        for (int i = y - x1; i <= y + x1; i++) {
            this->setPixel(x + y1, i, color);
            this->setPixel(x - y1, i, color);
        }
    }
}

void DisplayN18::drawCircle(int x, int y, int radius, unsigned short color) {
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x1 = 0;
    int y1 = radius;

    this->setPixel(x, y + radius, color);
    this->setPixel(x, y - radius, color);
    this->setPixel(x + radius, y, color);
    this->setPixel(x - radius, y, color);

    while (x1 < y1) {
        if (f >= 0) {
            y1--;
            ddF_y += 2;
            f += ddF_y;
        }

        x1++;
        ddF_x += 2;
        f += ddF_x;

        this->setPixel(x + x1, y + y1, color);
        this->setPixel(x - x1, y + y1, color);
        this->setPixel(x + x1, y - y1, color);
        this->setPixel(x - x1, y - y1, color);

        this->setPixel(x + y1, y + x1, color);
        this->setPixel(x - y1, y + x1, color);
        this->setPixel(x + y1, y - x1, color);
        this->setPixel(x - y1, y - x1, color);
    }
}

void DisplayN18::drawLine(int x0, int y0, int x1, int y1, unsigned short color) {
	if (x0 == x1) {
		if (y1 < y0) {
			int temp = y0;
			y0 = y1;
			y1 = temp;
		}

		this->setClippingArea((unsigned char)x0, (unsigned char)y0, 1, (unsigned char)y1 - y0 - 1);
		this->writeCommand(0x2C);

		unsigned short data[DisplayN18::STEP_Y];
		for (int i = 0; i < DisplayN18::STEP_Y; i++)
			data[i] = color;

		for (unsigned char thisY = y0; thisY < y1; thisY += DisplayN18::STEP_Y)
			this->writeData(reinterpret_cast<unsigned char*>(data), (y1 - thisY >= DisplayN18::STEP_Y ? DisplayN18::STEP_Y : y1 - thisY) * 2);

		return;
	}

	if (y0 == y1) {
		if (x1 < x0) {
			int temp = x0;
			x0 = x1;
			x1 = temp;
		}

		this->setClippingArea((unsigned char)x0, (unsigned char)x0, (unsigned char)x1 - x0 - 1, 1);
		this->writeCommand(0x2C);

		unsigned short data[DisplayN18::STEP_X];
		for (int i = 0; i < DisplayN18::STEP_X; i++)
			data[i] = color;

		for (unsigned char thisX = x0; thisX < x1; thisX += DisplayN18::STEP_X)
			this->writeData(reinterpret_cast<unsigned char*>(data), (x1 - thisX >= DisplayN18::STEP_X ? DisplayN18::STEP_X : x1 - thisX) * 2);

		return;
	}

	int t;
	bool steep = ((y1 - y0) < 0 ? -(y1 - y0) : (y1 - y0)) > ((x1 - x0) < 0 ? -(x1 - x0) : (x1 - x0));

	if (steep) {
		t = x0;
		x0 = y0;
		y0 = t;
		t = x1;
		x1 = y1;
		y1 = t;
	}

	if (x0 > x1) {
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

	for (; x0 < x1; x0++) {
		if (steep)
			this->setPixel(y0, x0, color);
		else
			this->setPixel(x0, y0, color);

		err -= dy;

		if (err < 0) {
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
	0x00,0x00,0x00,0x00,0x00, /* Space	0x20 */
	0x00,0x00,0x4f,0x00,0x00, /* ! */
	0x00,0x07,0x00,0x07,0x00, /* " */
	0x14,0x7f,0x14,0x7f,0x14, /* # */
	0x24,0x2a,0x7f,0x2a,0x12, /* $ */
	0x23,0x13,0x08,0x64,0x62, /* % */
	0x36,0x49,0x55,0x22,0x20, /* & */
	0x00,0x05,0x03,0x00,0x00, /* ' */
	0x00,0x1c,0x22,0x41,0x00, /* ( */
	0x00,0x41,0x22,0x1c,0x00, /* ) */
	0x14,0x08,0x3e,0x08,0x14, /* // */
	0x08,0x08,0x3e,0x08,0x08, /* + */
	0x50,0x30,0x00,0x00,0x00, /* , */
	0x08,0x08,0x08,0x08,0x08, /* - */
	0x00,0x60,0x60,0x00,0x00, /* . */
	0x20,0x10,0x08,0x04,0x02, /* / */
	0x3e,0x51,0x49,0x45,0x3e, /* 0		0x30 */
	0x00,0x42,0x7f,0x40,0x00, /* 1 */
	0x42,0x61,0x51,0x49,0x46, /* 2 */
	0x21,0x41,0x45,0x4b,0x31, /* 3 */
	0x18,0x14,0x12,0x7f,0x10, /* 4 */
	0x27,0x45,0x45,0x45,0x39, /* 5 */
	0x3c,0x4a,0x49,0x49,0x30, /* 6 */
	0x01,0x71,0x09,0x05,0x03, /* 7 */
	0x36,0x49,0x49,0x49,0x36, /* 8 */
	0x06,0x49,0x49,0x29,0x1e, /* 9 */
	0x00,0x36,0x36,0x00,0x00, /* : */
	0x00,0x56,0x36,0x00,0x00, /* ; */
	0x08,0x14,0x22,0x41,0x00, /* < */
	0x14,0x14,0x14,0x14,0x14, /* = */
	0x00,0x41,0x22,0x14,0x08, /* > */
	0x02,0x01,0x51,0x09,0x06, /* ? */
	0x3e,0x41,0x5d,0x55,0x1e, /* @		0x40 */
	0x7e,0x11,0x11,0x11,0x7e, /* A */
	0x7f,0x49,0x49,0x49,0x36, /* B */
	0x3e,0x41,0x41,0x41,0x22, /* C */
	0x7f,0x41,0x41,0x22,0x1c, /* D */
	0x7f,0x49,0x49,0x49,0x41, /* E */
	0x7f,0x09,0x09,0x09,0x01, /* F */
	0x3e,0x41,0x49,0x49,0x7a, /* G */
	0x7f,0x08,0x08,0x08,0x7f, /* H */
	0x00,0x41,0x7f,0x41,0x00, /* I */
	0x20,0x40,0x41,0x3f,0x01, /* J */
	0x7f,0x08,0x14,0x22,0x41, /* K */
	0x7f,0x40,0x40,0x40,0x40, /* L */
	0x7f,0x02,0x0c,0x02,0x7f, /* M */
	0x7f,0x04,0x08,0x10,0x7f, /* N */
	0x3e,0x41,0x41,0x41,0x3e, /* O */
	0x7f,0x09,0x09,0x09,0x06, /* P		0x50 */
	0x3e,0x41,0x51,0x21,0x5e, /* Q */
	0x7f,0x09,0x19,0x29,0x46, /* R */
	0x26,0x49,0x49,0x49,0x32, /* S */
	0x01,0x01,0x7f,0x01,0x01, /* T */
	0x3f,0x40,0x40,0x40,0x3f, /* U */
	0x1f,0x20,0x40,0x20,0x1f, /* V */
	0x3f,0x40,0x38,0x40,0x3f, /* W */
	0x63,0x14,0x08,0x14,0x63, /* X */
	0x07,0x08,0x70,0x08,0x07, /* Y */
	0x61,0x51,0x49,0x45,0x43, /* Z */
	0x00,0x7f,0x41,0x41,0x00, /* [ */
	0x02,0x04,0x08,0x10,0x20, /* \ */
	0x00,0x41,0x41,0x7f,0x00, /* ] */
	0x04,0x02,0x01,0x02,0x04, /* ^ */
	0x40,0x40,0x40,0x40,0x40, /* _ */
	0x00,0x00,0x03,0x05,0x00, /* `		0x60 */
	0x20,0x54,0x54,0x54,0x78, /* a */
	0x7F,0x44,0x44,0x44,0x38, /* b */
	0x38,0x44,0x44,0x44,0x44, /* c */
	0x38,0x44,0x44,0x44,0x7f, /* d */
	0x38,0x54,0x54,0x54,0x18, /* e */
	0x04,0x04,0x7e,0x05,0x05, /* f */
	0x08,0x54,0x54,0x54,0x3c, /* g */
	0x7f,0x08,0x04,0x04,0x78, /* h */
	0x00,0x44,0x7d,0x40,0x00, /* i */
	0x20,0x40,0x44,0x3d,0x00, /* j */
	0x7f,0x10,0x28,0x44,0x00, /* k */
	0x00,0x41,0x7f,0x40,0x00, /* l */
	0x7c,0x04,0x7c,0x04,0x78, /* m */
	0x7c,0x08,0x04,0x04,0x78, /* n */
	0x38,0x44,0x44,0x44,0x38, /* o */
	0x7c,0x14,0x14,0x14,0x08, /* p		0x70 */
	0x08,0x14,0x14,0x14,0x7c, /* q */
	0x7c,0x08,0x04,0x04,0x00, /* r */
	0x48,0x54,0x54,0x54,0x24, /* s */
	0x04,0x04,0x3f,0x44,0x44, /* t */
	0x3c,0x40,0x40,0x20,0x7c, /* u */
	0x1c,0x20,0x40,0x20,0x1c, /* v */
	0x3c,0x40,0x30,0x40,0x3c, /* w */
	0x44,0x28,0x10,0x28,0x44, /* x */
	0x0c,0x50,0x50,0x50,0x3c, /* y */
	0x44,0x64,0x54,0x4c,0x44, /* z */
	0x08,0x36,0x41,0x41,0x00, /* { */
	0x00,0x00,0x77,0x00,0x00, /* | */
	0x00,0x41,0x41,0x36,0x08, /* } */
	0x08,0x08,0x2a,0x1c,0x08  /* ~ */
};

void DisplayN18::drawCharacter(int x, int y, const char character, unsigned short foreColor, unsigned short backColor, unsigned char fontSize) {
	if (character > 126 || character < 32)
		return;

	unsigned short* horizontal = new unsigned short[DisplayN18::CHAR_HEIGHT * fontSize];
	for (int i = 0; i < DisplayN18::CHAR_WIDTH; i++) {
		for (int j = 0; j < DisplayN18::CHAR_HEIGHT; j++) {
			for (int k = 0; k < fontSize; k++)
#ifdef Arduino_h
				horizontal[j * fontSize + k] = pgm_read_byte_near(characters + (character - 32) * 5 + i) & (1 << j) ? foreColor : backColor;
#else
				horizontal[j * fontSize + k] = characters[character][i] & (1 << j) ? foreColor : backColor;
#endif
		}

		for (int k = 0; k < fontSize; k++)
			this->draw(horizontal, x + i * fontSize + k, y, 1, DisplayN18::CHAR_HEIGHT * fontSize);
	}

	for (int i = 0; i < DisplayN18::CHAR_HEIGHT; i++)
		for (int k = 0; k < fontSize; k++)
			horizontal[i * fontSize + k] = backColor;

	for (int k = 0; k < fontSize; k++)
		this->draw(horizontal, x + DisplayN18::CHAR_WIDTH * fontSize + k, y, 1, DisplayN18::CHAR_HEIGHT * fontSize);

	delete[] horizontal;
}

void DisplayN18::drawString(int x, int y, const char* str, unsigned short foreColor, unsigned short backColor, unsigned char fontSize) {
	if (*str == '\0')
		return;

	do {
		this->drawCharacter(x, y, *str, foreColor, backColor, fontSize);
		x += (DisplayN18::CHAR_WIDTH + DisplayN18::CHAR_SPACING) * fontSize;
	} while (*(++str) != '\0');
}
