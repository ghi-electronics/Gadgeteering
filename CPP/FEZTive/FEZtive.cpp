#include "FEZtive.h"

using namespace GHI;
using namespace GHI::Modules;

FEZtive::FEZtive(int socket)
{
	this->sock = mainboard->getSocket(socket);
	this->sock->ensureTypeIsSupported(GHI::Socket::Types::S);
}

void FEZtive::Initialize(int numLEDS, unsigned int spiClockRateKHZ)
{
	this->spi_config = new Interfaces::SPIConfiguration(true, 0, 0, false, true, spiClockRateKHZ);
	this->spi = new Interfaces::SPIDevice(mainboard->getSPIBus(this->sock),0,this->spi_config);

    LEDs = new Color[numLEDS];

	for (int i = 0; i < numLEDS; i++)
    {
        LEDs[i] = Color(0, 0, 0);
    }

    _zeros = new char[3 * ((numLEDS + 63) / 64)];
}

void FEZtive::SetAll(Color color)
{
	unsigned char colorarr[3];

	//Clear();

	spi->write((unsigned char*)_zeros, strlen(_zeros));

	for (int i = 0; i < ledLength; i += 4)
	{
		LEDs[i] = color;
		LEDs[i + 1] = color;
		LEDs[i + 2] = color;
		LEDs[i + 3] = color;
                
		this->GetColorForRender(LEDs[i], colorarr, colorarr + 1, colorarr + 2);
		spi->write(colorarr, 1);
		this->GetColorForRender(LEDs[i + 1], colorarr, colorarr + 1, colorarr + 2);
		spi->write(colorarr, 1);
		this->GetColorForRender(LEDs[i + 2], colorarr, colorarr + 1, colorarr + 2);
		spi->write(colorarr, 1);
		this->GetColorForRender(LEDs[i + 3], colorarr, colorarr + 1, colorarr + 2);
		spi->write(colorarr, 1);
	}

	spi->write((unsigned char*)_zeros, strlen(_zeros), true);
}

void FEZtive::SetAll(Color *colorArr)
{
	unsigned char color[3];

	spi->write((unsigned char*)_zeros, strlen(_zeros));

	for (int i = 0; i < ledLength; i += 4)
	{
		SetLED(colorArr[i], i);
		SetLED(colorArr[i + 1], i + 1);
		SetLED(colorArr[i + 2], i + 2);
		SetLED(colorArr[i + 3], i + 3); 
                
		this->GetColorForRender(LEDs[i], color, color + 1, color + 2);
		spi->write(color, 1);
		this->GetColorForRender(LEDs[i + 1], color, color + 1, color + 2);
		spi->write(color, 1);
		this->GetColorForRender(LEDs[i + 2], color, color + 1, color + 2);
		spi->write(color, 1);
		this->GetColorForRender(LEDs[i + 3], color, color + 1, color + 2);
		spi->write(color, 1);
	}

	spi->write((unsigned char*)_zeros, strlen(_zeros), true);
}

void FEZtive::SetLED(Color color, int numLED)
{
	LEDs[numLED] = color;

	Redraw();
}

Color *FEZtive::GetCurrentColors()
{
	return LEDs;
}

void FEZtive::Clear()
{
	SetAll(Color(0,0,0));
}

void FEZtive::Redraw()
{
	unsigned char color[3];

	spi->write((unsigned char*)_zeros, strlen(_zeros));

	for (int i = 0; i < ledLength; i += 4)
	{
		this->GetColorForRender(LEDs[i], color, color + 1, color + 2);
		spi->write(color, 1);
		this->GetColorForRender(LEDs[i + 1], color, color + 1, color + 2);
		spi->write(color, 1);
		this->GetColorForRender(LEDs[i + 2], color, color + 1, color + 2);
		spi->write(color, 1);
		this->GetColorForRender(LEDs[i + 3], color, color + 1, color + 2);
		spi->write(color, 1);
	}

	spi->write((unsigned char*)_zeros, strlen(_zeros), true);
}

void FEZtive::GetColorForRender(Color color, unsigned char* g, unsigned char* r, unsigned char* b)
{
	*g = (0x80 | color.green);
	*r = (0x80 | color.red);
	*b = (0x80 | color.blue);
}