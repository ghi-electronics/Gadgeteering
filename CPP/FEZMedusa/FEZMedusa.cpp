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

#include "Arduino.h"
#include "FEZMedusa.h"

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;

Mainboard* GHI::mainboard = NULL;

FEZMedusa::FEZMedusa() : FEZMedusaMini() {
	mainboard = this;

	Socket* socket = this->registerSocket(new Socket(5, Socket::Types::Y | Socket::Types::X | Socket::Types::P));
	socket->pins[3] = 0x00 | FEZMedusa::EXTENDER_MASK;
	socket->pins[4] = 0x01 | FEZMedusa::EXTENDER_MASK;
	socket->pins[5] = 0x02 | FEZMedusa::EXTENDER_MASK;
	socket->pins[6] = 0x03 | FEZMedusa::EXTENDER_MASK;
	socket->pins[7] = 0x60 | FEZMedusa::EXTENDER_MASK;
	socket->pins[8] = 0x61 | FEZMedusa::EXTENDER_MASK;
	socket->pins[9] = 0x62 | FEZMedusa::EXTENDER_MASK;

	socket = this->registerSocket(new Socket(6, Socket::Types::Y | Socket::Types::X | Socket::Types::P));
	socket->pins[3] = 0x04 | FEZMedusa::EXTENDER_MASK;
	socket->pins[4] = 0x05 | FEZMedusa::EXTENDER_MASK;
	socket->pins[5] = 0x06 | FEZMedusa::EXTENDER_MASK;
	socket->pins[6] = 0x07 | FEZMedusa::EXTENDER_MASK;
	socket->pins[7] = 0x63 | FEZMedusa::EXTENDER_MASK;
	socket->pins[8] = 0x64 | FEZMedusa::EXTENDER_MASK;
	socket->pins[9] = 0x65 | FEZMedusa::EXTENDER_MASK;

	socket = this->registerSocket(new Socket(7, Socket::Types::Y | Socket::Types::X | Socket::Types::P));
	socket->pins[3] = 0x10 | FEZMedusa::EXTENDER_MASK;
	socket->pins[4] = 0x11 | FEZMedusa::EXTENDER_MASK;
	socket->pins[5] = 0x12 | FEZMedusa::EXTENDER_MASK;
	socket->pins[6] = 0x13 | FEZMedusa::EXTENDER_MASK;
	socket->pins[7] = 0x66 | FEZMedusa::EXTENDER_MASK;
	socket->pins[8] = 0x67 | FEZMedusa::EXTENDER_MASK;
	socket->pins[9] = 0x70 | FEZMedusa::EXTENDER_MASK;
	
	socket = this->registerSocket(new Socket(8, Socket::Types::Y | Socket::Types::X | Socket::Types::P));
	socket->pins[3] = 0x14 | FEZMedusa::EXTENDER_MASK;
	socket->pins[4] = 0x15 | FEZMedusa::EXTENDER_MASK;
	socket->pins[5] = 0x16 | FEZMedusa::EXTENDER_MASK;
	socket->pins[6] = 0x17 | FEZMedusa::EXTENDER_MASK;
	socket->pins[7] = 0x71 | FEZMedusa::EXTENDER_MASK;
	socket->pins[8] = 0x72 | FEZMedusa::EXTENDER_MASK;
	socket->pins[9] = 0x73 | FEZMedusa::EXTENDER_MASK;

	socket = this->registerSocket(new Socket(9, Socket::Types::Y | Socket::Types::X | Socket::Types::P));
	socket->pins[3] = 0x20 | FEZMedusa::EXTENDER_MASK;
	socket->pins[4] = 0x21 | FEZMedusa::EXTENDER_MASK;
	socket->pins[5] = 0x22 | FEZMedusa::EXTENDER_MASK;
	socket->pins[6] = 0x23 | FEZMedusa::EXTENDER_MASK;
	socket->pins[7] = 0x74 | FEZMedusa::EXTENDER_MASK;
	socket->pins[8] = 0x75 | FEZMedusa::EXTENDER_MASK;
	socket->pins[9] = 0x76 | FEZMedusa::EXTENDER_MASK;

	socket = this->registerSocket(new Socket(10, Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = 0x30 | FEZMedusa::EXTENDER_MASK;
	socket->pins[4] = 0x31 | FEZMedusa::EXTENDER_MASK;
	socket->pins[5] = 0x32 | FEZMedusa::EXTENDER_MASK;
	socket->pins[6] = 0x33 | FEZMedusa::EXTENDER_MASK;
	socket->pins[7] = 0x34 | FEZMedusa::EXTENDER_MASK;
	socket->pins[8] = 0x35 | FEZMedusa::EXTENDER_MASK;
	socket->pins[9] = 0x36 | FEZMedusa::EXTENDER_MASK;

	socket = this->registerSocket(new Socket(11, Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = 0x40 | FEZMedusa::EXTENDER_MASK;
	socket->pins[4] = 0x41 | FEZMedusa::EXTENDER_MASK;
	socket->pins[5] = 0x42 | FEZMedusa::EXTENDER_MASK;
	socket->pins[6] = 0x43 | FEZMedusa::EXTENDER_MASK;
	socket->pins[7] = 0x44 | FEZMedusa::EXTENDER_MASK;
	socket->pins[8] = 0x45 | FEZMedusa::EXTENDER_MASK;
	socket->pins[9] = 0x46 | FEZMedusa::EXTENDER_MASK;

	socket = this->registerSocket(new Socket(12, Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = 0x50 | FEZMedusa::EXTENDER_MASK;
	socket->pins[4] = 0x51 | FEZMedusa::EXTENDER_MASK;
	socket->pins[5] = 0x52 | FEZMedusa::EXTENDER_MASK;
	socket->pins[6] = 0x53 | FEZMedusa::EXTENDER_MASK;
	socket->pins[7] = 0x54 | FEZMedusa::EXTENDER_MASK;
	socket->pins[8] = 0x55 | FEZMedusa::EXTENDER_MASK;
	socket->pins[9] = 0x56 | FEZMedusa::EXTENDER_MASK;

	this->extenderChip = new Modules::IO60P16(3);
}

FEZMedusa::~FEZMedusa() {
	delete this->extenderChip;
}

void FEZMedusa::setIOMode(CPUPin pinNumber, IOState state, ResistorMode resistorMode) {
	if (!(pinNumber & FEZMedusa::EXTENDER_MASK))
		FEZMedusaMini::setIOMode(pinNumber, state, resistorMode);
	else
		this->extenderChip->setIOMode(pinNumber & ~FEZMedusa::EXTENDER_MASK, state, resistorMode);
}

void FEZMedusa::setPWM(CPUPin pinNumber, double dutyCycle, double frequency) {
	!(pinNumber & FEZMedusa::EXTENDER_MASK) ? FEZMedusaMini::setPWM(pinNumber, dutyCycle, frequency) : this->extenderChip->setPWM(pinNumber & ~FEZMedusa::EXTENDER_MASK, dutyCycle, frequency);
}

bool FEZMedusa::readDigital(CPUPin pinNumber) {
	!(pinNumber & FEZMedusa::EXTENDER_MASK) ? FEZMedusaMini::readDigital(pinNumber) : this->extenderChip->readDigital(pinNumber & ~FEZMedusa::EXTENDER_MASK);
}

void FEZMedusa::writeDigital(CPUPin pinNumber, bool value) {
	!(pinNumber & FEZMedusa::EXTENDER_MASK) ? FEZMedusaMini::writeDigital(pinNumber, value) : this->extenderChip->writeDigital(pinNumber & ~FEZMedusa::EXTENDER_MASK, value);
}

double FEZMedusa::readAnalog(CPUPin pinNumber) {
	if (!(pinNumber & FEZMedusa::EXTENDER_MASK)) 
	{
		return FEZMedusaMini::readAnalog(pinNumber);
	}
	else
	{
		mainboard->panic(Exceptions::ERR_READ_ANALOG_NOT_SUPPORTED);
		return 0.0;
	}
}

void FEZMedusa::writeAnalog(CPUPin pinNumber, double voltage) {
	!(pinNumber & FEZMedusa::EXTENDER_MASK) ? FEZMedusaMini::writeAnalog(pinNumber, voltage) : mainboard->panic(Exceptions::ERR_WRITE_ANALOG_NOT_SUPPORTED);
}

FEZMedusaMini::FEZMedusaMini() {
	mainboard = this;
	
	Socket* socket = this->registerSocket(new Socket(1, Socket::Types::I | Socket::Types::S | Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = 7;
	socket->pins[4] = 8;
	socket->pins[5] = 9;
	socket->pins[6] = 10;
	socket->pins[7] = 11;
	socket->pins[8] = 12;
	socket->pins[9] = 13;

	socket = this->registerSocket(new Socket(2, Socket::Types::I | Socket::Types::P | Socket::Types::U | Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = 2;
	socket->pins[4] = 1;
	socket->pins[5] = 0;
	socket->pins[6] = 4;
	socket->pins[7] = 3;
	socket->pins[8] = 5;
	socket->pins[9] = 6;

	socket = this->registerSocket(new Socket(3, Socket::Types::A | Socket::Types::I | Socket::Types::X));
	socket->pins[3] = A0; //A0 = 14
	socket->pins[4] = A1; //A1 = 15
	socket->pins[5] = A2; //A2 = 16
	socket->pins[6] = A3; //A3 = 17
	socket->pins[8] = A4; //A4 = 18
	socket->pins[9] = A5; //A5 = 19
}

FEZMedusaMini::~FEZMedusaMini() {

}
				
void FEZMedusaMini::panic(unsigned char error, unsigned char specificError) {
	Serial.begin(9600);
	while (true) {
		Serial.print((int)error);
		Serial.print("-");
		Serial.println((int)specificError);
	}
}
				
void FEZMedusaMini::print(const char* toPrint) {
	Serial.begin(9600);
	Serial.print(toPrint);
}
				
void FEZMedusaMini::print(int toPrint) {
	Serial.begin(9600);
	Serial.print(toPrint);
}
				
void FEZMedusaMini::print(double toPrint) {
	Serial.begin(9600);
	Serial.print(toPrint);
}

void FEZMedusaMini::setIOMode(CPUPin pinNumber, IOState state, ResistorMode resistorMode) {
	if (state == IOStates::DIGITAL_INPUT)
		pinMode(pinNumber, resistorMode == ResistorModes::PULL_UP ? INPUT_PULLUP : INPUT);
	else if ((state == IOStates::DIGITAL_OUTPUT) || (state == IOStates::PWM))
		pinMode(pinNumber, OUTPUT);
}

void FEZMedusaMini::setPWM(CPUPin pinNumber, double dutyCycle, double frequency) {
	::analogWrite(pinNumber, static_cast<int>(dutyCycle * 255.0));
}

void FEZMedusaMini::setPWM(CPUPin pinNumber, double frequency, double dutyCycle, double duration)
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

bool FEZMedusaMini::readDigital(CPUPin pinNumber) {
	return ::digitalRead(pinNumber) == HIGH;
}

void FEZMedusaMini::writeDigital(CPUPin pinNumber, bool value) {
	::digitalWrite(pinNumber, value ? HIGH : LOW);
}

double FEZMedusaMini::readAnalog(CPUPin pinNumber) {
	return static_cast<double>(::analogRead(pinNumber)) / 1024 * 3.3;
}

double FEZMedusaMini::readAnalogProportion(CPUPin pinNumber) {
	return this->readAnalog(pinNumber) / 3.3;
}

void FEZMedusaMini::writeAnalog(CPUPin pinNumber, double voltage) {
	::analogWrite(pinNumber, voltage * (1024 / 3.3));
}

void FEZMedusaMini::writeAnalogProportion(CPUPin pinNumber, double proportion) {
	this->writeAnalog(pinNumber, proportion * 3.3);
}

Interfaces::SPIBus* FEZMedusaMini::getSPIBus(CPUPin mosi, CPUPin miso, CPUPin sck) {
	for (SPIBus* current = (SPIBus*)this->spiBusses.startV(); !this->spiBusses.ended(); current = (SPIBus*)this->spiBusses.nextV())
        if (current->mosi == mosi && current->miso == miso && current->sck == sck)
            return (GHI::Interfaces::SPIBus*)current;

	SPIBus* bus = new FEZMedusaMini::SPIBus(mosi, miso, sck);
	this->spiBusses.addV(bus);
	return bus;
}

Interfaces::SerialDevice* FEZMedusaMini::getSerialDevice(unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits, CPUPin txPin, CPUPin rxPin) {
    for (SerialDevice* current = (SerialDevice*)this->serialDevices.startV(); !this->serialDevices.ended(); current = (SerialDevice*)this->serialDevices.nextV())
        if (current->tx == txPin && current->rx == rxPin)
            return current;

	SerialDevice* bus = new FEZMedusaMini::SerialDevice(txPin, rxPin, baudRate, parity, stopBits, dataBits);
	this->serialDevices.addV(bus);
	return bus;
}

Interfaces::I2CBus* FEZMedusaMini::getI2CBus(CPUPin sdaPin, CPUPin sclPin) {
    for (I2CBus* current = (I2CBus*)this->i2cBusses.startV(); !this->i2cBusses.ended(); current = (I2CBus*)this->i2cBusses.nextV())
        if (current->scl == sclPin && current->sda == sdaPin)
            return current;
		
	I2CBus* bus = new FEZMedusaMini::I2CBus(sdaPin, sclPin);
	this->i2cBusses.addV(bus);
	return bus;
}
