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

#include "../Gadgeteering/Gadgeteering.h"

#include "FEZLynx.h"

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;

#define I2C_DELAY() ;

#ifdef CLOCK_STRETCH
#define WAIT_SCL() while (!readSCL()) ;
#else
#define WAIT_SCL() releaseSCL();
#endif

FEZLynx::I2CBus::I2CBus(CPUPin sda, CPUPin scl) : Interfaces::I2CBus(sda, scl)
{
    this->startSent = false;
    this->readSCL();
    this->readSDA();
}

FEZLynx::I2CBus::~I2CBus()
{

}

void FEZLynx::I2CBus::clearSCL()
{
    mainboard->setIOMode(this->scl, IOStates::DIGITAL_OUTPUT);
}

void FEZLynx::I2CBus::releaseSCL()
{
    mainboard->setIOMode(this->scl, IOStates::DIGITAL_INPUT, ResistorModes::PULL_UP);
}

bool FEZLynx::I2CBus::readSCL()
{
    mainboard->setIOMode(this->scl, IOStates::DIGITAL_INPUT, ResistorModes::PULL_UP);
    return mainboard->readDigital(this->scl);
}

void FEZLynx::I2CBus::clearSDA()
{
    mainboard->setIOMode(this->sda, IOStates::DIGITAL_OUTPUT);
}

void FEZLynx::I2CBus::releaseSDA()
{
    mainboard->setIOMode(this->sda, IOStates::DIGITAL_INPUT, ResistorModes::PULL_UP);
}

bool FEZLynx::I2CBus::readSDA()
{
    mainboard->setIOMode(this->sda, IOStates::DIGITAL_INPUT, ResistorModes::PULL_UP);
    return mainboard->readDigital(this->sda);
}

void FEZLynx::I2CBus::writeBit(bool bit)
{
    if (bit)
        releaseSDA();
    else
        clearSDA();
	
	WAIT_SCL();

	clearSCL();
}

bool FEZLynx::I2CBus::readBit()
{
    releaseSDA();
	
	WAIT_SCL();

    bool bit = readSDA();
	
	clearSCL();

    return bit;
}

void FEZLynx::I2CBus::sendStartCondition()
{
	if (startSent) {
		releaseSDA();

		WAIT_SCL();
	}

	releaseSDA();

	clearSDA();

	clearSCL();

	startSent = true;
}

void FEZLynx::I2CBus::sendStopCondition()
{
    clearSDA();
	
	WAIT_SCL();

	releaseSDA();

	startSent = false;
}

bool FEZLynx::I2CBus::transmit(bool sendStart, bool sendStop, unsigned char data) {
	if (sendStart)
		sendStartCondition();
	
    for (unsigned char mask = 0x80; mask != 0x00; mask >>= 1)
		writeBit(data & mask);

	bool nack = readBit();

	if (sendStop)
		sendStopCondition();

	return nack;
}

unsigned char FEZLynx::I2CBus::receive(bool sendAcknowledgeBit, bool sendStop)
{
	unsigned char bit, d = 0;

	for (bit = 0; bit < 8; bit++)
		d = (d << 1) | readBit();

	writeBit(!sendAcknowledgeBit);
	
	if (sendStop)
		sendStopCondition();

	return d;
}

unsigned int FEZLynx::I2CBus::write(const unsigned char* buffer, unsigned int count, unsigned char address, bool sendStop)
{
    if (!count) 
		return 0;

	unsigned int numWrite = 0;
	unsigned int i = 0;
	
	if (!this->transmit(true, false, address))
		for (i = 0; i < count - 1; i++)
			if (!this->transmit(false, false, buffer[i]))
				numWrite++;
	
	if (!this->transmit(false, sendStop, buffer[i]))
		numWrite++;
	
	return numWrite;
 }

unsigned int FEZLynx::I2CBus::read(unsigned char* buffer, unsigned int count, unsigned char address, bool sendStop)
{
    if (!count) 
		return 0;

	unsigned int numRead = 0;
	unsigned int i = 0;

	if (!this->transmit(true, false, address | 1)) {
		for (i = 0; i < count - 1; i++) {
			buffer[i] = this->receive(true, false);
			numRead++;
		}
	}

    buffer[i] = this->receive(false, sendStop);
	numRead++;

    return numRead;
}

bool FEZLynx::I2CBus::writeRead(const unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength, unsigned int* numWritten, unsigned int* numRead, unsigned char address)
{
    *numWritten = 0;
	*numRead = 0;

	unsigned int i = 0;
	unsigned int write = 0;
	unsigned int read = 0;

    if (writeLength > 0) {
		if (!this->transmit(true, false, address)) {
			for (i = 0; i < writeLength - 1; i++) {
				if (!this->transmit(false, false, writeBuffer[i])) {
					(write)++;
				}
			}
		}

		if (!this->transmit(false, (readLength == 0), writeBuffer[i]))
			write++; 

		*numWritten = write;
    }

    if (readLength > 0) {
		if (!this->transmit(true, false, address | 1)) {
			for (i = 0; i < readLength - 1; i++) {
				readBuffer[i] = this->receive(true, false);
				read++;
			}
		}

		readBuffer[i] = this->receive(false, true);
		read++;
		*numRead = read;
    }

	*numRead = read = readLength;

	return (write + read) == (writeLength + readLength);
}

void FEZLynx::I2CBus::SetChannel(FT_HANDLE i2cChannel)
{
    this->channel = i2cChannel;
}
