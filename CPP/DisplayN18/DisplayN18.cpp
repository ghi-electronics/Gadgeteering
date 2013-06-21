#include "DisplayN18.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

DisplayN18::DisplayN18(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::X);

	  this->resetPin = new DigitalOutput(socket, Socket::Pins::Three, false);
	  this->backlightPin = new DigitalOutput(socket, Socket::Pins::Four, true);
	  this->rsPin = new DigitalOutput(socket, Socket::Pins::Five, false);
	  this->spi = socket->getSPIDevice(new SPIConfiguration(false, 0, 0, false, true, 4000), Socket::Pins::Six);
  
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

	this->clearScreen();
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

void DisplayN18::clearScreen(unsigned short color) {
	this->fillRect(0, 0, DisplayN18::WIDTH, DisplayN18::HEIGHT, color);
}

void DisplayN18::drawRaw(const unsigned char* data, int x, int y, int width, int height) {
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
	this->drawRaw(reinterpret_cast<unsigned char*>(&color), x, y, 1, 1);
}

void DisplayN18::fillRect(int x, int y, int width, int height, unsigned short color) {
	unsigned short data[DisplayN18::STEP_X * DisplayN18::STEP_Y];
	for (int i = 0; i < DisplayN18::STEP_X * DisplayN18::STEP_Y; i++)
		data[i] = color;
   
	unsigned char drawWidth = 0, drawHeight = 0;
	for (unsigned char thisX = x; thisX < x + width; thisX += DisplayN18::STEP_X) {
		for (unsigned char thisY = y; thisY < y + height; thisY += DisplayN18::STEP_Y) {
			drawWidth = x + width - thisX >= DisplayN18::STEP_X ? DisplayN18::STEP_X : x + width - thisX;
			drawHeight = y + height - thisY >= DisplayN18::STEP_Y ? DisplayN18::STEP_Y : y + height - thisY;

			this->drawRaw(reinterpret_cast<unsigned char*>(data), thisX, thisY, drawWidth, drawHeight);
		}
	}
}

void DisplayN18::drawRect(int x, int y, int width, int height, unsigned short color) {
	for (unsigned char i = x; i < x + width; i++) {
        this->setPixel(i, y, color);
        this->setPixel(i, y + height - 1, color);
    }

    for (unsigned char i = y; i < y + height; i++) {
        this->setPixel(x, i, color);
        this->setPixel(x + width - 1, i, color);
    }
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
		