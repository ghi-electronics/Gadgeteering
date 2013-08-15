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

	ledLength = numLEDS;
    LEDs = new Color[numLEDS];

	for (int i = 0; i < numLEDS; i++)
    {
        LEDs[i] = Color(0, 0, 0);
    }

	_zeroLength = 3 * ((numLEDS + 63) / 64);
	_zeros = new char[_zeroLength];

	for(int i = 0; i < strlen(_zeros); i++)
	{
		_zeros[i] = 0x00;
	}
}

void FEZtive::SetAll(Color color)
{
	unsigned char colorarr[3];

	for(int i = 0; i < 3; i++)
		colorarr[i] = 0;

	//Clear();

	spi->write((unsigned char*)_zeros,1);

	for (int i = 0; i < ledLength; i += 2)
	{
		LEDs[i] = color;
		LEDs[i + 1] = color;

		this->GetColorForRender(LEDs[i], colorarr + 2, colorarr + 1, colorarr);
		spi->write(colorarr, 3);
		this->GetColorForRender(LEDs[i + 1], colorarr + 2, colorarr + 1, colorarr);
		spi->write(colorarr, 3);
	}

	spi->write((unsigned char*)_zeros, 1, true);
}

void FEZtive::SetAll(Color *colorArr)
{
	unsigned char color[3];

	spi->write((unsigned char*)_zeros, strlen(_zeros));

	for (int i = 0; i < ledLength; i += 2)
	{
		SetLED(colorArr[i], i);
		SetLED(colorArr[i + 1], i + 1);
                
		this->GetColorForRender(LEDs[i], color, color + 1, color + 2);
		spi->write(color, 1);
		this->GetColorForRender(LEDs[i + 1], color, color + 1, color + 2);
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

	for (int i = 0; i < ledLength; i += 2)
	{
		this->GetColorForRender(LEDs[i], color, color + 1, color + 2);
		spi->write(color, 1);
		this->GetColorForRender(LEDs[i + 1], color, color + 1, color + 2);
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