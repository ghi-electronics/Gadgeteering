#include "SoftwareI2C.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

#define I2C_DELAY() ;

SoftwareI2C::SoftwareI2C(unsigned char address, CPUPin sda, CPUPin scl) {
	this->address = address << 1;

	this->scl = scl;
	this->sda = sda;
	
	this->start = false;
	this->readSCL();
	this->readSDA();
}

SoftwareI2C::SoftwareI2C(unsigned char address, Socket* socket, Socket::Pin sdaPinNumber, Socket::Pin sclPinNumber) {
	this->address = address << 1;

	this->scl = socket->pins[sclPinNumber];
	this->sda = socket->pins[sdaPinNumber];
	
	this->start = false;
	this->readSCL();
	this->readSDA();
}

void SoftwareI2C::clearSCL() {
	mainboard->setIOMode(this->scl, IOStates::DIGITAL_OUTPUT);
	mainboard->writeDigital(this->scl, false);
}

bool SoftwareI2C::readSCL() {
	mainboard->setIOMode(this->scl, IOStates::DIGITAL_INPUT, ResistorModes::PULL_UP);
	return mainboard->readDigital(this->scl);
}

void SoftwareI2C::clearSDA() {
	mainboard->setIOMode(this->sda, IOStates::DIGITAL_OUTPUT);
	mainboard->writeDigital(this->sda, false);
}

bool SoftwareI2C::readSDA() {
	mainboard->setIOMode(this->sda, IOStates::DIGITAL_INPUT, ResistorModes::PULL_UP);
	return mainboard->readDigital(this->sda);
}

bool SoftwareI2C::writeBit(bool bit) {
    if (bit)
		this->readSDA();
    else
		this->clearSDA();
	
    I2C_DELAY();

	unsigned long endTime = System::TimeElapsed() + 5000;
	while (!this->readSCL() && System::TimeElapsed() < endTime)
		;
	
    if (bit && !this->readSDA())
		return false;
	
    I2C_DELAY();
	this->clearSCL();

    return true;
}

bool SoftwareI2C::readBit() {
    this->readSDA();
	
    I2C_DELAY();
	
	unsigned long endTime = System::TimeElapsed() + 5000;
	while (!this->readSCL() && System::TimeElapsed() < endTime)
		;

    bool bit = this->readSDA();
	
    I2C_DELAY();
	this->clearSCL();

    return bit;
}

bool SoftwareI2C::sendStartCondition() {
	if (this->start) {
		this->readSDA();
		I2C_DELAY();

		unsigned long endTime = System::TimeElapsed() + 5000;
		while (!this->readSCL() && System::TimeElapsed() < endTime)
			;

	}

    if (!this->readSDA())
		return false;

	this->clearSDA();
	I2C_DELAY();
	this->clearSCL();

	this->start = true;

	return true;
}

bool SoftwareI2C::sendStopCondition() {
	this->clearSDA();
	I2C_DELAY();

	unsigned long endTime = System::TimeElapsed() + 5000;
	while (!this->readSCL() && System::TimeElapsed() < endTime)
		;

	if (!this->readSDA())
		return false;
	
	I2C_DELAY();
	this->start = false;

	return true;
}

bool SoftwareI2C::transmit(bool sendStart, bool sendStop, unsigned char data) {
	unsigned int bit;
	bool nack;
	
	if (sendStart)
		this->sendStartCondition();
	
    for (bit = 0; bit < 8; bit++) {
		this->writeBit((data & 0x80) != 0);

		data <<= 1;
    }
    
    nack = this->readBit();

	if (sendStop)
		this->sendStopCondition();
	
     return nack;
}

unsigned char SoftwareI2C::receive(bool sendAcknowledgeBit, bool sendStopCondition) {
	unsigned char d = 0;
	unsigned int bit = 0;

	for (bit = 0; bit < 8; bit++) {
		d <<= 1;

		if (this->readBit())
			d |= 1;
	}
	
	this->writeBit(!sendAcknowledgeBit);

	if (sendStopCondition)
		this->sendStopCondition();

	return d;
}

unsigned int SoftwareI2C::writeBytes(const unsigned char* data, unsigned int length, bool sendStop) {
	if (!length) 
		return 0;

	unsigned int numWrite = 0;
	unsigned int i = 0;
	
	if (!this->transmit(true, false, this->address))
		for (i = 0; i < length - 1; i++)
			if (!this->transmit(false, false, data[i]))
				numWrite++;
	
	if (!this->transmit(false, sendStop, data[i]))
		numWrite++;
	
	return numWrite;
 }

unsigned int SoftwareI2C::readBytes(unsigned char* data, unsigned int length) {	
	if (!length) 
		return 0;

	unsigned int numRead = 0;
	unsigned int i = 0;

	if (!this->transmit(true, false, this->address | 1)) {
		for (i = 0; i < length - 1; i++) {
			data[i] = this->receive(true, false);
			numRead++;
		}
	}

    data[i] = this->receive(false, true);
	numRead++;

    return numRead;
}

bool SoftwareI2C::writeRead(const unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength, unsigned int* numWritten, unsigned int* numRead) {
	*numWritten = 0;
	*numRead = 0;

	unsigned int i = 0;
	unsigned char write = 0;
	unsigned char read = 0;

    if (writeLength > 0) {
		if (!this->transmit(true, false, this->address)) {
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
		if (!this->transmit(true, false, this->address | 1)) {
			for (i = 0; i < readLength - 1; i++) {
				readBuffer[i] = this->receive(true, false);
				read++;
			}
		}

		readBuffer[i] = this->receive(false, true);
		read++;
		*numRead = read;
    }

	return (write + read) == (writeLength + readLength);
}

bool SoftwareI2C::writeRegister(unsigned char registerAddress, unsigned char value) {
	unsigned char data[2] = {registerAddress, value};
	return this->writeBytes(data, 2) == 2;
}

unsigned char SoftwareI2C::readRegister(unsigned char registerAddress) {
	unsigned char value = 0xFF;
	unsigned int written, read;

	this->writeRead(&registerAddress, 1, &value, 1, &written, &read);

	return value;
}