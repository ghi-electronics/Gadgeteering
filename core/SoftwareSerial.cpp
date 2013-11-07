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

SoftwareSerial::~SoftwareSerial()
{
}

unsigned int SoftwareSerial::read(unsigned char* buffer, unsigned int count)
{
	int bitDelay = this->bitPeriod - System::CyclesToMicroseconds(50);

	while(rxPort->read()); //Wait for start bit

	if(rxPort->read() == false) //Check to see if we got here due to line noise
	{
		for(unsigned int i = 0; i < count; i++)
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

unsigned int SoftwareSerial::available()
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