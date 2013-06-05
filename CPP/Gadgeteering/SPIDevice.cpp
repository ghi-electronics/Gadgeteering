#include "SPIDevice.hpp"
#include "SPIBus.hpp"
#include "Interfaces.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

SPIDevice::SPIDevice(SPIBus *spiBus, Socket* socket, Socket::Pin chipSelectPin) {
	socket->ensureTypeIsSupported(Socket::Types::S);

	this->chipSelect = new DigitalOutput(socket, chipSelectPin, true);

	bus = spiBus;
}

SPIDevice::~SPIDevice() {
	delete this->chipSelect;
}

char SPIDevice::writeReadByte(char toSend, bool deselectChip) 
{ 
	this->chipSelect->write(this->bus->configuration->chipSelectActiveState);
	this->bus->writeReadByte(toSend);
	if (deselectChip)
		this->chipSelect->write(!this->bus->configuration->chipSelectActiveState);
}

void SPIDevice::writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count, bool deselectChip) 
{
	this->chipSelect->write(this->bus->configuration->chipSelectActiveState);
	this->bus->writeAndRead(sendBuffer, receiveBuffer, count);
	if (deselectChip)
		this->chipSelect->write(!this->bus->configuration->chipSelectActiveState);
}

void SPIDevice::writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, bool deselectChip) 
{
	this->chipSelect->write(this->bus->configuration->chipSelectActiveState);
	this->bus->writeThenRead(sendBuffer, receiveBuffer, sendCount, receiveCount);
	if (deselectChip)
		this->chipSelect->write(!this->bus->configuration->chipSelectActiveState);
}

void SPIDevice::write(char* buffer, unsigned int count, bool deselectChip) 
{ 
	this->chipSelect->write(this->bus->configuration->chipSelectActiveState);
	this->bus->write(buffer, count);
	if (deselectChip)
		this->chipSelect->write(!this->bus->configuration->chipSelectActiveState);
}

void SPIDevice::read(char* buffer, unsigned int count, bool deselectChip) 
{
	this->chipSelect->write(this->bus->configuration->chipSelectActiveState);
	this->bus->read(buffer, count);
	if (deselectChip)
		this->chipSelect->write(!this->bus->configuration->chipSelectActiveState);
}