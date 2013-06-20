#include "Socket.hpp"
#include "Mainboard.hpp"
#include "I2CBus.hpp"
#include "SPIBus.hpp"
#include "SerialDevice.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

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
		this->i2cBus = mainboard->getI2CBus(this, Pins::Eight, Pins::Nine);

	return this->i2cBus;
}

SPIBus* Socket::getSPIBus() {
	if (!this->spiBus)
		this->spiBus = mainboard->getSPIBus(this, Pins::Eight, Pins::Nine);

	return this->spiBus;
}

I2CDevice* Socket::getI2CDevice(unsigned char address) {
	return this->getI2CBus()->getI2CDevice(address);
}

SerialDevice* Socket::getSerialDevice(unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits){
	return mainboard->getSerialDevice(baudRate, parity, stopBits, dataBits, this, Pins::Four, Pins::Five);
}

SPIDevice* Socket::getSPIDevice(SPIConfiguration* configuration, Pin chipSelectPinNumber){
	return this->getSPIBus()->getSPIDevice(chipSelectPinNumber, configuration);
}
