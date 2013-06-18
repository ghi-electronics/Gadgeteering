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
			int bitDelay = this->bitPeriod - System::CyclesToMicroseconds(50);

			while(rx->read()); //Wait for start bit

			if(rx->read() == false) //Check to see if we got here due to line noise
			{
				for(int i = 0; i < bytes; i++)
				{
					System::SleepMicro(bitDelay / 2 - System::CyclesToMicroseconds(50));

					for(int offset = 0; offset < 8; offset++)
					{
						System::SleepMicro(bitDelay);

						data[i] |= (rx->read() ? 1 : 0) << offset;
					}
				}
			}

			System::SleepMicro(bitDelay);
			return data;
		}

		void SoftwareSerial::write(const char* data, int offset, int count)
		{
			for(int i = offset; i < count; i++)
				this->write(data[i]);
		}

		void SoftwareSerial::write(const char data)
		{
			int bitDelay = this->bitPeriod - System::CyclesToMicroseconds(50);

			tx->write(false);
			System::SleepMicro(bitDelay);

			for(unsigned char mask = 0x01; mask; mask <<= 1)
			{
				if(mask & data)
					tx->write(true);
				else
					tx->write(false);

				System::SleepMicro(bitDelay);
			}

			tx->write(true);
			System::SleepMicro(bitDelay);
		}

		char SoftwareSerial::available()
		{
			this->receive();
		}

		void SoftwareSerial::open()
		{
		}

		void SoftwareSerial::close()
		{
		}

		void SoftwareSerial::receive()
		{
		}

		bool SoftwareSerial::overflow()
		{
			return false;
		}
	}
}