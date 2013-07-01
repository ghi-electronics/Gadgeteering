#include "Types.hpp"

using namespace GHI;

Color::Color()
{
	this->red = 0x00;
	this->green = 0x00;
	this->blue = 0x00;
}

Color::Color(unsigned char r, unsigned char g, unsigned char b)
{
	this->red = r;
	this->green = g;
	this->blue = b;
}

void Color::Set(unsigned char r, unsigned char g, unsigned char b)
{
	this->red = r;
	this->green = g;
	this->blue = b;
}

void Color::Set(Color color)
{
	this->Set(color.red, color.green, color.blue);
}
		
const Color Color::RED(0xFF, 0, 0);
const Color Color::GREEN(0, 0xFF, 0);
const Color Color::BLUE(0, 0, 0xFF);
const Color Color::WHITE(0xFF, 0xFF, 0xFF);
const Color Color::BLACK(0, 0, 0);