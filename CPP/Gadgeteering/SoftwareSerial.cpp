#include "SoftwareSerial.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

SoftwareSerial::SoftwareSerial(CPUPin txPin, CPUPin rxPin, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits) : SerialDevice(rxPin, rxPin, baudRate, parity, stopBits, dataBits)
{
	this->rxPort = new DigitalInput(txPin, ResistorModes::FLOATING);
	this->txPort = new DigitalOutput(rxPin, false);

	//Calculate baudrate
	this->bitPeriod = 1000000 / this->baudRate;

	//Null out buffer
	for(int i = 0; i < 64; i++)
		buffer[i] = 0x00;
}

unsigned int SoftwareSerial::read(unsigned char* buffer, unsigned int count)
{
	int bitDelay = this->bitPeriod - System::CyclesToMicroseconds(50);

	while(rxPort->read()); //Wait for start bit

	if(rxPort->read() == false) //Check to see if we got here due to line noise
	{
		for(int i = 0; i < count; i++)
		{
			System::SleepMicro(bitDelay / 2 - System::CyclesToMicroseconds(50));

			for(int offset = 0; offset < 8; offset++)
			{
				System::SleepMicro(bitDelay);

				buffer[i] |= (rxPort->read() ? 1 : 0) << offset;
			}
		}
	}

	System::SleepMicro(bitDelay);
	return count;
}

void SoftwareSerial::write(const unsigned char* buffer, unsigned int count)
{
	for(unsigned int i = 0; i < count; i++)
		this->write(buffer[i]);
}

void SoftwareSerial::write(const char* buffer, unsigned int count)
{
	for(unsigned int i = 0; i < count; i++)
		this->write(buffer[i]);
}

void SoftwareSerial::write(const char data)
{
	int bitDelay = this->bitPeriod - System::CyclesToMicroseconds(50);

	txPort->write(false);
	System::SleepMicro(bitDelay);

	for(unsigned char mask = 0x01; mask; mask <<= 1)
	{
		if(mask & data)
			txPort->write(true);
		else
			txPort->write(false);

		System::SleepMicro(bitDelay);
	}

	txPort->write(true);
	System::SleepMicro(bitDelay);
}

unsigned char SoftwareSerial::available()
{
	this->receive();

	return false;
}

void SoftwareSerial::open()
{
}

void SoftwareSerial::close()
{
}

unsigned char SoftwareSerial::receive()
{
	return 0x00;
}

bool SoftwareSerial::overflow()
{
	return false;
}