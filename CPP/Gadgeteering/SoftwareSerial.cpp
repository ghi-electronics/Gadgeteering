#include "SoftwareSerial.hpp"

namespace GHI
{
	namespace Interfaces
	{
		SoftwareSerial::SoftwareSerial(CPUPin rxPin, CPUPin txPin, int baudrate)
		{
			this->baud = baudrate;
			this->rx = new DigitalInput(rxPin, ResistorModes::FLOATING);
			this->tx = new DigitalOutput(txPin, false);

			//Calculate baudrate
			this->bitPeriod = 1000000 / this->baud;

			//Null out buffer
			for(int i = 0; i < 64; i++)
				buffer[i] = '\n';
		}

		char* SoftwareSerial::read(char bytes)
		{
			char* data = new char[bytes];

			//Null out buffer
			for(int i = 0; i < bytes; i++)
				data[i] = 0x00;

			while(rx->read()); //Wait for start bit

			if(rx->read() == false) //Check to see if we got here due to line noise
			{
				delayMicroseconds(bitDelay / 2 - clockCyclesToMicroseconds(50));

				for(int i = 0; i < bytes; i++)
				{
					for(int offset = 0; offset < 8; offset++)
						data[i] |= (rx->read() ? 1 : 0) << offset;
				}
			}

			return data;
		}

		char SoftwareSerial::available()
		{
			this->receive();
		}
	}
}