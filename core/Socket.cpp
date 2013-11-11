/*
Copyright 2013 Gadgeteering Electronics LLC

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

#include "Socket.h"
#include "Mainboard.h"
#include "I2CBus.h"
#include "SPIBus.h"
#include "SerialDevice.h"

using namespace Gadgeteering;
using namespace Gadgeteering::Interfaces;

Socket::Socket(unsigned char number, Type type) : number(number), type(type) {
	this->spiBus = NULL;
	this->i2cBus = NULL;

	for (unsigned char i = 0; i < 10; i++)
		this->pins[i] = Socket::Pins::Unconnected;
}

void Socket::ensureTypeIsSupported(Type type) {
	if ((this->type & type) == 0)
		mainboard->panic(Exceptions::ERR_SOCKET_NOT_HAVE_TYPE);
}

I2CBus* Socket::getI2CBus() {
	if (!this->i2cBus)
		this->i2cBus = mainboard->getI2CBus(this);

	return this->i2cBus;
}

SPIBus* Socket::getSPIBus() {
	if (!this->spiBus)
		this->spiBus = mainboard->getSPIBus(this);

	return this->spiBus;
}

I2CDevice* Socket::getI2CDevice(unsigned char address) {
	return this->getI2CBus()->getI2CDevice(address);
}

SerialDevice* Socket::getSerialDevice(unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits){
	return mainboard->getSerialDevice(baudRate, parity, stopBits, dataBits, this);
}

SPIDevice* Socket::getSPIDevice(SPIConfiguration* configuration, Pin chipSelectPinNumber){
	return this->getSPIBus()->getSPIDevice(this->pins[chipSelectPinNumber], configuration);
}
