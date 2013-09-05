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

#include "Mainboard.hpp"
#include "Module.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

Mainboard::Mainboard() {
	if (mainboard != NULL)
		mainboard->panic(Exceptions::ERR_ONLY_ONE_MAINBOARD);
}

Mainboard::~Mainboard() {
	for (Socket* current = (Socket*)this->sockets.startV(); !this->sockets.ended(); current = (Socket*)this->sockets.nextV())
		delete current;
	for (I2CBus* current = (I2CBus*)this->i2cBusses.startV(); !this->i2cBusses.ended(); current = (I2CBus*)this->i2cBusses.nextV())
		delete current;
	for (SPIBus* current = (SPIBus*)this->spiBusses.startV(); !this->spiBusses.ended(); current = (SPIBus*)this->spiBusses.nextV())
		delete current;
	for (SerialDevice* current = (SerialDevice*)this->serialDevices.startV(); !this->serialDevices.ended(); current = (SerialDevice*)this->serialDevices.nextV())
		delete current;
}

void Mainboard::panic(unsigned char error, unsigned char specificError) {
	while (true)
		;
}

void Mainboard::print(const char* toPrint) {
	mainboard->panic(Exceptions::ERR_NOT_IMPLEMENTED);
}

void Mainboard::print(int toPrint) {
	mainboard->panic(Exceptions::ERR_NOT_IMPLEMENTED);
}

void Mainboard::print(double toPrint) {
	mainboard->panic(Exceptions::ERR_NOT_IMPLEMENTED);
}

Socket* Mainboard::registerSocket(Socket* socket) {
	this->sockets.addV(socket);

	return socket;
}

Socket* Mainboard::getSocket(unsigned char number) {
	for (Socket* current = (Socket*)this->sockets.startV(); !this->sockets.ended(); current = (Socket*)this->sockets.nextV())
		if (current->number == number)
			return current;

	return NULL;
}

void Mainboard::setPWM(CPUPin pin, double dutyCycle, double frequency) { mainboard->panic(Exceptions::ERR_PWM_NOT_SUPPORTED); }
bool Mainboard::readDigital(CPUPin pin) { mainboard->panic(Exceptions::ERR_READ_DIGITAL_NOT_SUPPORTED); return false; }
void Mainboard::writeDigital(CPUPin pin, bool value) { mainboard->panic(Exceptions::ERR_WRITE_DIGITAL_NOT_SUPPORTED); }
double Mainboard::readAnalog(CPUPin pin) { mainboard->panic(Exceptions::ERR_READ_ANALOG_NOT_SUPPORTED); return 0.0; }
double Mainboard::readAnalogProportion(CPUPin pin) { mainboard->panic(Exceptions::ERR_READ_ANALOG_NOT_SUPPORTED); return 0.0; }
void Mainboard::writeAnalog(CPUPin pin, double voltage) { mainboard->panic(Exceptions::ERR_WRITE_ANALOG_NOT_SUPPORTED); }
void Mainboard::writeAnalogProportion(CPUPin pin, double voltage) { mainboard->panic(Exceptions::ERR_WRITE_ANALOG_NOT_SUPPORTED); }
void Mainboard::setIOMode(CPUPin pin, IOState state, ResistorMode resistorMode) { mainboard->panic(Exceptions::ERR_SET_IO_NOT_SUPPORTED); }

GHI::Interfaces::SerialDevice* Mainboard::getSerialDevice(unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits, CPUPin txPin, CPUPin rxPin) { mainboard->panic(Exceptions::ERR_SERIAL_NOT_SUPPORTED); return NULL; }
GHI::Interfaces::SPIBus* Mainboard::getSPIBus(CPUPin mosiPin, CPUPin misoPin, CPUPin sckPin) { mainboard->panic(Exceptions::ERR_SPI_NOT_SUPPORTED); return NULL; }
GHI::Interfaces::I2CBus* Mainboard::getI2CBus(CPUPin sdaPin, CPUPin sclPin) { mainboard->panic(Exceptions::ERR_I2C_NOT_SUPPORTED); return NULL; }

GHI::Interfaces::SerialDevice* Mainboard::getSerialDevice(unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits, Socket* socket, Socket::Pin txPinNumber, Socket::Pin rxPinNumber) { 
	return this->getSerialDevice(baudRate, parity, stopBits, dataBits, socket->pins[txPinNumber], socket->pins[rxPinNumber]);
}

GHI::Interfaces::SPIBus* Mainboard::getSPIBus(Socket* socket, Socket::Pin mosiPinNumber, Socket::Pin misoPinNumber, Socket::Pin sckPinNumber) { 
	return this->getSPIBus(socket->pins[mosiPinNumber], socket->pins[misoPinNumber], socket->pins[sckPinNumber]);
}

GHI::Interfaces::I2CBus* Mainboard::getI2CBus(Socket* socket, Socket::Pin sdaPinNumber, Socket::Pin sclPinNumber) { 
	return this->getI2CBus(socket->pins[sdaPinNumber], socket->pins[sclPinNumber]);
}
