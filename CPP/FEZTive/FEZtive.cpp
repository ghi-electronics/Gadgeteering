#include "FEZtive.h"

namespace GHI
{
	namespace Modules
	{
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
			//Clear();

			spi->write((unsigned char*)_zeros, strlen(_zeros));

			for (int i = 0; i < ledLength; i += 4)
			{
				LEDs[i] = color;
				LEDs[i + 1] = color;
				LEDs[i + 2] = color;
				LEDs[i + 3] = color;
                
				spi->write((unsigned char*)LEDs[i].GetForRender(),1);
				spi->write((unsigned char*)LEDs[i + 1].GetForRender(),1);
				spi->write((unsigned char*)LEDs[i + 2].GetForRender(),1);
				spi->write((unsigned char*)LEDs[i + 3].GetForRender(),1);
			}

			spi->write((unsigned char*)_zeros, strlen(_zeros), true);
		}

		void FEZtive::SetAll(Color *colorArr)
		{
			spi->write((unsigned char*)_zeros, strlen(_zeros));

			for (int i = 0; i < ledLength; i += 4)
			{
				SetLED(colorArr[i], i);
				SetLED(colorArr[i + 1], i + 1);
				SetLED(colorArr[i + 2], i + 2);
				SetLED(colorArr[i + 3], i + 3); 
                
				spi->write((unsigned char*)LEDs[i].GetForRender(),1);
				spi->write((unsigned char*)LEDs[i + 1].GetForRender(),1);
				spi->write((unsigned char*)LEDs[i + 2].GetForRender(),1);
				spi->write((unsigned char*)LEDs[i + 3].GetForRender(),1);
			}

			spi->write((unsigned char*)_zeros, strlen(_zeros), true);
		}

		void FEZtive::SetLED(Color color, int numLED)
		{
			LEDs[numLED] = color;

			Redraw();
		}

		FEZtive::Color *FEZtive::GetCurrentColors()
		{
			return LEDs;
		}

		void FEZtive::Clear()
		{
			SetAll(Color(0,0,0));
		}

		void FEZtive::Redraw()
		{
			spi->write((unsigned char*)_zeros, strlen(_zeros));

			for (int i = 0; i < ledLength; i += 4)
			{
				spi->write((unsigned char*)LEDs[i].GetForRender(),1);
				spi->write((unsigned char*)LEDs[i + 1].GetForRender(),1);
				spi->write((unsigned char*)LEDs[i + 2].GetForRender(),1);
				spi->write((unsigned char*)LEDs[i + 3].GetForRender(),1);
			}

			spi->write((unsigned char*)_zeros, strlen(_zeros), true);
		}

		//Helper Classes
		FEZtive::Color::Color()
		{
			this->red = 0x00;
			this->green = 0x00;
			this->blue = 0x00;
		}

		FEZtive::Color::Color(char r, char g, char b)
		{
			this->red = r;
			this->green = g;
			this->blue = b;
		}

		void FEZtive::Color::Set(char r, char g, char b)
		{
			this->red = r;
			this->green = g;
			this->blue = b;
		}

		void FEZtive::Color::Set(Color color)
		{
			this->Set(color.red, color.green, color.blue);
		}

		char *FEZtive::Color::GetForRender()
		{
			char *render = new char[3];
			render[0] = (0x80 | this->green);
			render[1] = (0x80 | this->red);
			render[2] = (0x80 | this->blue);

			return render;
		}
	}
}