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

#include <Arduino.h>
#include "FEZMedusa3D.h"

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;

Mainboard* GHI::mainboard = NULL;

FEZMedusa3D::FEZMedusa3D() {
	mainboard = this;
	
	Socket* socket = this->registerSocket(new Socket(1, Socket::Types::I | Socket::Types::S | Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = A5;
	socket->pins[4] = A3;
	socket->pins[5] = A4;
	socket->pins[6] = 22;
	socket->pins[7] = 8;
	socket->pins[8] = 9;
	socket->pins[9] = 10;

	socket = this->registerSocket(new Socket(2, Socket::Types::I | Socket::Types::P | Socket::Types::U | Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = A8;
	socket->pins[4] = A7;
	socket->pins[5] = A6;
	socket->pins[6] = 23;
	socket->pins[7] = 5;
	socket->pins[8] = 6;
	socket->pins[9] = 7;

	socket = this->registerSocket(new Socket(3, Socket::Types::A | Socket::Types::P | Socket::Types::Y));
	socket->pins[3] = A11;
	socket->pins[4] = A10;
	socket->pins[5] = A9;
	socket->pins[6] = 24;
	socket->pins[7] = 2;
	socket->pins[8] = 3;
	socket->pins[9] = 4;

	socket = this->registerSocket(new Socket(4, Socket::Types::I | Socket::Types::U | Socket::Types::X));
	socket->pins[3] = 25;
	socket->pins[4] = 1;
	socket->pins[5] = 0;
	socket->pins[6] = 26;
	socket->pins[7] = -1;
	socket->pins[8] = 20;
	socket->pins[9] = 21;

	socket = this->registerSocket(new Socket(5, Socket::Types::I | Socket::Types::U | Socket::Types::X));
	socket->pins[3] = 27;
	socket->pins[4] = 14;
	socket->pins[5] = 15;
	socket->pins[6] = 28;
	socket->pins[7] = -1;
	socket->pins[8] = 20;
	socket->pins[9] = 21;

	socket = this->registerSocket(new Socket(6, Socket::Types::I | Socket::Types::U | Socket::Types::X));
	socket->pins[3] = 39;
	socket->pins[4] = 16;
	socket->pins[5] = 17;
	socket->pins[6] = 38;
	socket->pins[7] = -1;
	socket->pins[8] = 20;
	socket->pins[9] = 21;

	socket = this->registerSocket(new Socket(7, Socket::Types::I | Socket::Types::U | Socket::Types::X));
	socket->pins[3] = 46;
	socket->pins[4] = 18;
	socket->pins[5] = 19;
	socket->pins[6] = 47;
	socket->pins[7] = -1;
	socket->pins[8] = 20;
	socket->pins[9] = 21;

	socket = this->registerSocket(new Socket(8, Socket::Types::S | Socket::Types::C | Socket::Types::X));
	socket->pins[3] = 44;
	socket->pins[4] = 69;
	socket->pins[5] = 68;
	socket->pins[6] = 45;
	socket->pins[7] = 75;
	socket->pins[8] = 74;
	socket->pins[9] = 76;

	socket = this->registerSocket(new Socket(9, Socket::Types::S | Socket::Types::X));
	socket->pins[3] = 41;
	socket->pins[4] = 40;
	socket->pins[5] = 43;
	socket->pins[6] = 42;
	socket->pins[7] = 75;
	socket->pins[8] = 74;
	socket->pins[9] = 76;

	socket = this->registerSocket(new Socket(10, Socket::Types::O | Socket::Types::Y));
	socket->pins[3] = 33;
	socket->pins[4] = 32;
	socket->pins[5] = 67;
	socket->pins[6] = 34;
	socket->pins[7] = 36;
	socket->pins[8] = 35;
	socket->pins[9] = 37;

	socket = this->registerSocket(new Socket(11, Socket::Types::O | Socket::Types::X));
	socket->pins[3] = 31;
	socket->pins[4] = 30;
	socket->pins[5] = 66;
	socket->pins[6] = -1;
	socket->pins[7] = -1;
	socket->pins[8] = -1;
	socket->pins[9] = -1;

	socket = this->registerSocket(new Socket(12, Socket::Types::I | Socket::Types::P | Socket::Types::U | Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = A1;
	socket->pins[4] = A2;
	socket->pins[5] = A0;
	socket->pins[6] = 29;
	socket->pins[7] = 11;
	socket->pins[8] = 12;
	socket->pins[9] = 13;
}

FEZMedusa3D::~FEZMedusa3D() {

}
				
void FEZMedusa3D::panic(unsigned char error, unsigned char specificError) {
	Serial.begin(9600);
	while (true) 
	{
		Serial.print((int)error);
		Serial.print("-");
		Serial.println((int)specificError);

		System::Sleep(100);
	}
}
				
void FEZMedusa3D::print(const char* toPrint) {
	Serial.begin(9600);
	Serial.print(toPrint);
}
				
void FEZMedusa3D::print(int toPrint) {
	Serial.begin(9600);
	Serial.print(toPrint);
}
				
void FEZMedusa3D::print(double toPrint) 
{
	Serial.begin(9600);
	Serial.print(toPrint);
}

void FEZMedusa3D::setIOMode(CPUPin pinNumber, IOState state, ResistorMode resistorMode) {
	if (state == IOStates::DIGITAL_INPUT)
		::pinMode(pinNumber, resistorMode == ResistorModes::PULL_UP ? INPUT_PULLUP : INPUT);
	else if ((state == IOStates::DIGITAL_OUTPUT) || (state == IOStates::PWM_OUTPUT))
		::pinMode(pinNumber, OUTPUT);
}

void FEZMedusa3D::setPWM(CPUPin pinNumber, double dutyCycle, double frequency) {
	::analogWrite(pinNumber, static_cast<int>(dutyCycle * 255.0));
}

void FEZMedusa3D::setPWM(CPUPin pinNumber, double frequency, double dutyCycle, double duration)
{
	if (frequency <= 0 || dutyCycle < 0 || dutyCycle > 1)
		return;
  
	double periodUS = 1000000 / frequency;
	unsigned long sleepHigh = (unsigned long)(periodUS * dutyCycle);
	unsigned long sleepLow = (unsigned long)(periodUS * (1 - dutyCycle));
	unsigned long endTime = System::TimeElapsed() + (unsigned long)(duration * 1000);
	
	::pinMode(pinNumber, OUTPUT);
	do {
		::digitalWrite(pinNumber, HIGH);
		::delayMicroseconds(sleepHigh * 1.59);
		::digitalWrite(pinNumber, LOW);
		::delayMicroseconds(sleepLow * 1.59);
	} while (endTime > System::TimeElapsed());
}

bool FEZMedusa3D::readDigital(CPUPin pinNumber) {
	return ::digitalRead(pinNumber) == HIGH;
}

void FEZMedusa3D::writeDigital(CPUPin pinNumber, bool value) 
{
	if(pinNumber == 255)
		return;

	::digitalWrite(pinNumber, value ? HIGH : LOW);
}

double FEZMedusa3D::readAnalog(CPUPin pinNumber) 
{
	return static_cast<double>(::analogRead(pinNumber)) / 1024 * 3.3;
}

double FEZMedusa3D::readAnalogProportion(CPUPin pinNumber) 
{
	return this->readAnalog(pinNumber) / 3.3;
}

void FEZMedusa3D::writeAnalog(CPUPin pinNumber, double voltage) 
{
	::analogWrite(pinNumber, voltage * (1024 / 3.3));
}

void FEZMedusa3D::writeAnalogProportion(CPUPin pinNumber, double proportion) {
	this->writeAnalog(pinNumber, proportion * 3.3);
}

Interfaces::SPIBus* FEZMedusa3D::getSPIBus(CPUPin mosi, CPUPin miso, CPUPin sck) {
	for (SPIBus* current = (SPIBus*)this->spiBusses.startV(); !this->spiBusses.ended(); current = (SPIBus*)this->spiBusses.nextV())
        if (current->mosi == mosi && current->miso == miso && current->sck == sck)
            return (GHI::Interfaces::SPIBus*)current;

	SPIBus* bus = new FEZMedusa3D::SPIBus(mosi, miso, sck);
	this->spiBusses.addV(bus);
	return bus;
	
	return NULL;
}

Interfaces::SerialDevice* FEZMedusa3D::getSerialDevice(unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits, CPUPin txPin, CPUPin rxPin) {
    for (SerialDevice* current = (SerialDevice*)this->serialDevices.startV(); !this->serialDevices.ended(); current = (SerialDevice*)this->serialDevices.nextV())
        if (current->tx == txPin && current->rx == rxPin)
            return current;

	SerialDevice* bus = new FEZMedusa3D::SerialDevice(txPin, rxPin, baudRate, parity, stopBits, dataBits);
	this->serialDevices.addV(bus);
	return bus;
}

Interfaces::I2CBus* FEZMedusa3D::getI2CBus(CPUPin sdaPin, CPUPin sclPin, bool isHardware) {
    for (I2CBus* current = (I2CBus*)this->i2cBusses.startV(); !this->i2cBusses.ended(); current = (I2CBus*)this->i2cBusses.nextV())
        if (current->scl == sclPin && current->sda == sdaPin)
            return current;
		
	I2CBus* bus = new FEZMedusa3D::I2CBus(sdaPin, sclPin);
	this->i2cBusses.addV(bus);
	return bus;
}

void GHI::System::Sleep(unsigned long time)
{
	delay(time);
}

void GHI::System::SleepMicro(unsigned long time)
{
	delayMicroseconds(time * 1.59);
}

unsigned long GHI::System::TimeElapsed()
{
	return micros();
}

unsigned long GHI::System::CyclesToMicroseconds(unsigned long val)
{
	return clockCyclesToMicroseconds(val);
}

void GHI::System::RandomNumberSeed(int seed)
{
	randomSeed(seed);
}

int GHI::System::RandomNumber(int low, int max)
{
	return random(low, max);
}

#ifdef GADGETEERING_HARDWARE_I2C
#include <Wire.h>
#endif

#ifndef GADGETEERING_HARDWARE_I2C
#define I2C_DELAY() ;
#endif

FEZMedusa3D::I2CBus::I2CBus(CPUPin sda, CPUPin scl) : Interfaces::I2CBus(sda, scl)
{
#ifndef GADGETEERING_HARDWARE_I2C
	this->start = false;
	this->readSCL();
	this->readSDA();
#else
	Wire.begin();
#endif
}

FEZMedusa3D::I2CBus::~I2CBus()
{
#ifdef GADGETEERING_HARDWARE_I2C
	Wire.end();
#endif
}

#ifndef GADGETEERING_HARDWARE_I2C
void FEZMedusa3D::I2CBus::clearSCL()
{
	mainboard->setIOMode(this->scl, IOStates::DIGITAL_OUTPUT);
	mainboard->writeDigital(this->scl, false);
}

bool FEZMedusa3D::I2CBus::readSCL()
{
	mainboard->setIOMode(this->scl, IOStates::DIGITAL_INPUT, ResistorModes::PULL_UP);
	return mainboard->readDigital(this->scl);
}

void FEZMedusa3D::I2CBus::clearSDA()
{
	mainboard->setIOMode(this->sda, IOStates::DIGITAL_OUTPUT);
	mainboard->writeDigital(this->sda, false);
}

bool FEZMedusa3D::I2CBus::readSDA()
{
	mainboard->setIOMode(this->sda, IOStates::DIGITAL_INPUT, ResistorModes::PULL_UP);
	return mainboard->readDigital(this->sda);
}

bool FEZMedusa3D::I2CBus::writeBit(bool bit)
{
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

bool FEZMedusa3D::I2CBus::readBit()
{
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

bool FEZMedusa3D::I2CBus::sendStartCondition()
{
	if (this->start)
	{
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

bool FEZMedusa3D::I2CBus::sendStopCondition()
{
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

bool FEZMedusa3D::I2CBus::transmit(bool sendStart, bool sendStop, unsigned char data)
{
	unsigned char bit;
	bool nack;

	if (sendStart)
		this->sendStartCondition();

	for (bit = 0; bit < 8; bit++)
	{
		this->writeBit((data & 0x80) != 0);

		data <<= 1;
	}

	nack = this->readBit();

	if (sendStop)
		this->sendStopCondition();

	return nack;
}

unsigned char FEZMedusa3D::I2CBus::receive(bool sendAcknowledgeBit, bool sendStopCondition)
{
	unsigned char d = 0;
	unsigned char bit = 0;

	for (bit = 0; bit < 8; bit++)
	{
		d <<= 1;

		if (this->readBit())
			d |= 1;
	}

	this->writeBit(!sendAcknowledgeBit);

	if (sendStopCondition)
		this->sendStopCondition();

	return d;
}
#endif

unsigned int FEZMedusa3D::I2CBus::write(const unsigned char* buffer, unsigned int count, unsigned char address, bool sendStop)
{
#ifndef GADGETEERING_HARDWARE_I2C
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
#else
	Wire.beginTransmission(address);
	Wire.write(buffer, count);

	if (sendStop)
		Wire.endTransmission();
#endif
}

unsigned int FEZMedusa3D::I2CBus::read(unsigned char* buffer, unsigned int count, unsigned char address, bool sendStop)
{
#ifndef GADGETEERING_HARDWARE_I2C
	if (!count)
		return 0;

	unsigned int numRead = 0;
	unsigned int i = 0;

	if (!this->transmit(true, false, address | 1))
	{
		for (i = 0; i < count - 1; i++)
		{
			buffer[i] = this->receive(true, false);
			numRead++;
		}
	}

	buffer[i] = this->receive(false, sendStop);
	numRead++;

	return numRead;
#else
	Wire.requestFrom(address, count, sendStop);

	for (int i = 0; i < count; i++)
	{
		while (Wire.available() < 1) //Wait for one byte to avoid overflowing the buffer
			System::Sleep(10);

		buffer[i] = Wire.read();
	}
#endif
}

bool FEZMedusa3D::I2CBus::writeRead(const unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength, unsigned int* numWritten, unsigned int* numRead, unsigned char address)
{
#ifndef GADGETEERING_HARDWARE_I2C
	*numWritten = 0;
	*numRead = 0;

	unsigned int i = 0;
	unsigned int write = 0;
	unsigned int read = 0;

	if (writeLength > 0)
	{
		if (!this->transmit(true, false, address))
		{
			for (i = 0; i < writeLength - 1; i++)
			{
				if (!this->transmit(false, false, writeBuffer[i]))
				{
					(write)++;
				}
			}
		}

		if (!this->transmit(false, (readLength == 0), writeBuffer[i]))
			write++;

		*numWritten = write;
	}

	if (readLength > 0)
	{
		if (!this->transmit(true, false, address | 1))
		{
			for (i = 0; i < readLength - 1; i++)
			{
				readBuffer[i] = this->receive(true, false);
				read++;
			}
		}

		readBuffer[i] = this->receive(false, true);
		read++;
		*numRead = read;
	}

	return (write + read) == (writeLength + readLength);
#else
	this->write(writeBuffer, writeLength, address, false);
	this->read(readBuffer, readLength, address, true);
#endif
}


FEZMedusa3D::SerialDevice::SerialDevice(CPUPin tx, CPUPin rx, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits) : Interfaces::SerialDevice(tx, rx, baudRate, parity, stopBits, dataBits)
{

}

FEZMedusa3D::SerialDevice::~SerialDevice()
{

}

void FEZMedusa3D::SerialDevice::open()
{
	Serial.begin(this->baudRate);
}

void FEZMedusa3D::SerialDevice::close()
{
	Serial.end();
}

void FEZMedusa3D::SerialDevice::write(const unsigned char* buffer, unsigned int count)
{
	Serial.write(buffer, count);
	Serial.flush();
}

unsigned int FEZMedusa3D::SerialDevice::read(unsigned char* buffer, unsigned int count)
{
	return Serial.readBytes(reinterpret_cast<char*>(buffer), count);
}


unsigned int FEZMedusa3D::SerialDevice::available()
{
	return Serial.available();
}

#include <SPI.h>

//#define SYSTEM_CLOCK 84000U /*KHz*/
//#define GADGETEERING_SYSTEM_CLOCK 84000U /*KHz*/
//#define GADGETEERING_EXTENDED_SPI

FEZMedusa3D::SPIBus::SPIBus(CPUPin mosi, CPUPin miso, CPUPin sck) : Interfaces::SPIBus(mosi, miso, sck)
{
#ifndef GADGETEERING_EXTENDED_SPI
	this->spi = new SPIClass();
	this->spi->begin();
#else
	SPI.begin(0);
#endif
}

FEZMedusa3D::SPIBus::~SPIBus()
{
#ifndef GADGETEERING_EXTENDED_SPI
	this->spi->end();
	delete this->spi;
#else
	SPI.end();
#endif
}

void FEZMedusa3D::SPIBus::setup(GHI::Interfaces::SPIConfiguration* configuration)
{
#ifndef GADGETEERING_EXTENDED_SPI
	if (!configuration->clockIdleState && configuration->clockEdge)
		this->spi->setDataMode(SPI_MODE0);
	else if (!configuration->clockIdleState && !configuration->clockEdge)
		this->spi->setDataMode(SPI_MODE1);
	else if (configuration->clockIdleState && !configuration->clockEdge)
		this->spi->setDataMode(SPI_MODE2);
	else if (configuration->clockIdleState && configuration->clockEdge)
		this->spi->setDataMode(SPI_MODE3);

	unsigned int divider = SYSTEM_CLOCK / configuration->clockRate;
	unsigned char count = 1;

	while ((divider >>= 1) > 0)
		count++;

	switch (count)
	{
		case 1: mainboard->panic(Exceptions::ERR_SPI_NOT_SUPPORTED); break;
		case 2: this->spi->setClockDivider(SPI_CLOCK_DIV2); break;
		case 3: this->spi->setClockDivider(SPI_CLOCK_DIV4); break;
		case 4: this->spi->setClockDivider(SPI_CLOCK_DIV8); break;
		case 5: this->spi->setClockDivider(SPI_CLOCK_DIV16); break;
		case 6: this->spi->setClockDivider(SPI_CLOCK_DIV32); break;
		case 7: this->spi->setClockDivider(SPI_CLOCK_DIV64); break;
		case 8: this->spi->setClockDivider(SPI_CLOCK_DIV128); break;
	}
#else

	if (!configuration->clockIdleState && configuration->clockEdge)
		SPI.setDataMode(configuration->chipSelect, SPI_MODE0);
	else if (!configuration->clockIdleState && !configuration->clockEdge)
		SPI.setDataMode(configuration->chipSelect, SPI_MODE1);
	else if (configuration->clockIdleState && !configuration->clockEdge)
		SPI.setDataMode(configuration->chipSelect, SPI_MODE2);
	else if (configuration->clockIdleState && configuration->clockEdge)
		SPI.setDataMode(configuration->chipSelect, SPI_MODE3);

	unsigned int divider = GADGETEERING_SYSTEM_CLOCK / configuration->clockRate;
	SPI.setClockDivider(0, divider);
#endif
}

void FEZMedusa3D::SPIBus::selectChip(GHI::Interfaces::SPIConfiguration* configuration)
{
	mainboard->writeDigital(configuration->chipSelect, configuration->chipSelectActiveState);
	System::Sleep(configuration->chipSelectSetupTime);
}

void FEZMedusa3D::SPIBus::deselectChip(GHI::Interfaces::SPIConfiguration* configuration)
{
	System::Sleep(configuration->chipSelectHoldTime);
	mainboard->writeDigital(configuration->chipSelect, !configuration->chipSelectActiveState);
}

void FEZMedusa3D::SPIBus::writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, Interfaces::SPIConfiguration* configuration, bool deselectAfter)
{
	this->setup(configuration);
	this->selectChip(configuration);

#ifndef GADGETEERING_EXTENDED_SPI
	for (int i = 0; i < count; i++)
	{
		if (sendBuffer != NULL && receiveBuffer != NULL)
			receiveBuffer[i] = this->spi->transfer(sendBuffer[i]);
		else if (sendBuffer != NULL && receiveBuffer == NULL)
			this->spi->transfer(sendBuffer[i]);
		else if (sendBuffer == NULL && receiveBuffer != NULL)
			receiveBuffer[i] = this->spi->transfer(0);
	}
#else
	for (int i = 0; i < count; i++)
	{
		if (sendBuffer != NULL && receiveBuffer != NULL)
			receiveBuffer[i] = SPI.transfer(0, sendBuffer[i], SPI_CONTINUE);
		else if (sendBuffer != NULL && receiveBuffer == NULL)
			SPI.transfer(0, sendBuffer[i], SPI_CONTINUE);
		else if (sendBuffer == NULL && receiveBuffer != NULL)
			receiveBuffer[i] = SPI.transfer(0, 0, SPI_CONTINUE);
	}
#endif

	if (deselectAfter)
		this->deselectChip(configuration);
}