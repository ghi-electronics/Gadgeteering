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
#include "FEZAthena.h"

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;

Mainboard* GHI::mainboard = NULL;

FEZAthena::FEZAthena() {
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

	socket = this->registerSocket(new Socket(4, Socket::Types::I | Socket::Types::S | Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = 7;
	socket->pins[4] = 8;
	socket->pins[5] = 9;
	socket->pins[6] = 10;
	socket->pins[7] = 11;
	socket->pins[8] = 12;
	socket->pins[9] = 13;

	socket = this->registerSocket(new Socket(5, Socket::Types::I | Socket::Types::P | Socket::Types::U | Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = 2;
	socket->pins[4] = 1;
	socket->pins[5] = 0;
	socket->pins[6] = 4;
	socket->pins[7] = 3;
	socket->pins[8] = 5;
	socket->pins[9] = 6;

	socket = this->registerSocket(new Socket(6, Socket::Types::I | Socket::Types::S | Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = 7;
	socket->pins[4] = 8;
	socket->pins[5] = 9;
	socket->pins[6] = 10;
	socket->pins[7] = 11;
	socket->pins[8] = 12;
	socket->pins[9] = 13;

	socket = this->registerSocket(new Socket(7, Socket::Types::I | Socket::Types::P | Socket::Types::U | Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = 2;
	socket->pins[4] = 1;
	socket->pins[5] = 0;
	socket->pins[6] = 4;
	socket->pins[7] = 3;
	socket->pins[8] = 5;
	socket->pins[9] = 6;

	socket = this->registerSocket(new Socket(8, Socket::Types::I | Socket::Types::S | Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = 7;
	socket->pins[4] = 8;
	socket->pins[5] = 9;
	socket->pins[6] = 10;
	socket->pins[7] = 11;
	socket->pins[8] = 12;
	socket->pins[9] = 13;

	socket = this->registerSocket(new Socket(9, Socket::Types::I | Socket::Types::P | Socket::Types::U | Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = 2;
	socket->pins[4] = 1;
	socket->pins[5] = 0;
	socket->pins[6] = 4;
	socket->pins[7] = 3;
	socket->pins[8] = 5;
	socket->pins[9] = 6;

	socket = this->registerSocket(new Socket(10, Socket::Types::I | Socket::Types::S | Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = 7;
	socket->pins[4] = 8;
	socket->pins[5] = 9;
	socket->pins[6] = 10;
	socket->pins[7] = 11;
	socket->pins[8] = 12;
	socket->pins[9] = 13;

	socket = this->registerSocket(new Socket(11, Socket::Types::I | Socket::Types::P | Socket::Types::U | Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = 2;
	socket->pins[4] = 1;
	socket->pins[5] = 0;
	socket->pins[6] = 4;
	socket->pins[7] = 3;
	socket->pins[8] = 5;
	socket->pins[9] = 6;
}

FEZAthena::~FEZAthena() {

}
				
void FEZAthena::panic(unsigned char error, unsigned char specificError) {
	Serial.begin(9600);
	while (true) 
	{
		Serial.print((int)error);
		Serial.print("-");
		Serial.println((int)specificError);

		System::Sleep(100);
	}
}
				
void FEZAthena::print(const char* toPrint) {
	Serial.begin(9600);
	Serial.print(toPrint);
}
				
void FEZAthena::print(int toPrint) {
	Serial.begin(9600);
	Serial.print(toPrint);
}
				
void FEZAthena::print(double toPrint) 
{
	Serial.begin(9600);
	Serial.print(toPrint);
}

void FEZAthena::setIOMode(CPUPin pinNumber, IOState state, ResistorMode resistorMode) {
	if (state == IOStates::DIGITAL_INPUT)
		::pinMode(pinNumber, resistorMode == ResistorModes::PULL_UP ? INPUT_PULLUP : INPUT);
	else if ((state == IOStates::DIGITAL_OUTPUT) || (state == IOStates::PWM_OUTPUT))
		::pinMode(pinNumber, OUTPUT);
}

void FEZAthena::setPWM(CPUPin pinNumber, double dutyCycle, double frequency) {
	::analogWrite(pinNumber, static_cast<int>(dutyCycle * 255.0));
}

void FEZAthena::setPWM(CPUPin pinNumber, double frequency, double dutyCycle, double duration)
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

bool FEZAthena::readDigital(CPUPin pinNumber) {
	return ::digitalRead(pinNumber) == HIGH;
}

void FEZAthena::writeDigital(CPUPin pinNumber, bool value) 
{
	::digitalWrite(pinNumber, value ? HIGH : LOW);
}

double FEZAthena::readAnalog(CPUPin pinNumber) 
{
	return static_cast<double>(::analogRead(pinNumber)) / 1024 * 3.3;
}

double FEZAthena::readAnalogProportion(CPUPin pinNumber) 
{
	return this->readAnalog(pinNumber) / 3.3;
}

void FEZAthena::writeAnalog(CPUPin pinNumber, double voltage) 
{
	::analogWrite(pinNumber, voltage * (1024 / 3.3));
}

void FEZAthena::writeAnalogProportion(CPUPin pinNumber, double proportion) {
	this->writeAnalog(pinNumber, proportion * 3.3);
}

Interfaces::SPIBus* FEZAthena::getSPIBus(CPUPin mosi, CPUPin miso, CPUPin sck) {
	for (SPIBus* current = (SPIBus*)this->spiBusses.startV(); !this->spiBusses.ended(); current = (SPIBus*)this->spiBusses.nextV())
        if (current->mosi == mosi && current->miso == miso && current->sck == sck)
            return (GHI::Interfaces::SPIBus*)current;

	SPIBus* bus = new FEZAthena::SPIBus(mosi, miso, sck);
	this->spiBusses.addV(bus);
	return bus;
	
	return NULL;
}

Interfaces::SerialDevice* FEZAthena::getSerialDevice(unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits, CPUPin txPin, CPUPin rxPin) {
    for (SerialDevice* current = (SerialDevice*)this->serialDevices.startV(); !this->serialDevices.ended(); current = (SerialDevice*)this->serialDevices.nextV())
        if (current->tx == txPin && current->rx == rxPin)
            return current;

	SerialDevice* bus = new FEZAthena::SerialDevice(txPin, rxPin, baudRate, parity, stopBits, dataBits);
	this->serialDevices.addV(bus);
	return bus;
}

Interfaces::I2CBus* FEZAthena::getI2CBus(CPUPin sdaPin, CPUPin sclPin) {
    for (I2CBus* current = (I2CBus*)this->i2cBusses.startV(); !this->i2cBusses.ended(); current = (I2CBus*)this->i2cBusses.nextV())
        if (current->scl == sclPin && current->sda == sdaPin)
            return current;
		
	I2CBus* bus = new FEZAthena::I2CBus(sdaPin, sclPin);
	this->i2cBusses.addV(bus);
	return bus;
}
