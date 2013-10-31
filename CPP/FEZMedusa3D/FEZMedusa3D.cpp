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
