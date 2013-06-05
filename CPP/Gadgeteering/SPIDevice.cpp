#include "SPIDevice.hpp"
#include "SPIBus.hpp"
#include "Interfaces.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

SPIDevice::Configuration::Configuration(bool chipSelectActiveState, unsigned int chipSelectSetupTime, unsigned int chipSelectHoldTime, bool clockIdleState, bool clockEdge, unsigned int clockRate) {
	this->chipSelectActiveState = chipSelectActiveState;
	this->chipSelectSetupTime = chipSelectSetupTime;
	this->chipSelectHoldTime = chipSelectHoldTime;
	this->clockIdleState = clockIdleState;
	this->clockEdge = clockEdge;
	this->clockRate = clockRate;
}

SPIDevice::SPIDevice(SPIBus *spiBus, Socket* socket, Socket::Pin chipSelectPin, SPIDevice::Configuration* configuration) {
	socket->ensureTypeIsSupported(Socket::Types::S);

	this->chipSelect = new DigitalOutput(socket, chipSelectPin, true);
	this->configuration = configuration;

	bus = spiBus;
}

SPIDevice::~SPIDevice() {
	delete this->chipSelect;
}

char SPIDevice::writeReadByte(char toSend, bool deselectChip) 
{ 
	return bus->writeReadByte(toSend,deselectChip);
}

void SPIDevice::writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count, bool deselectChip) 
{
	bus->writeAndRead(sendBuffer,receiveBuffer,count,deselectChip);
}

void SPIDevice::writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, bool deselectChip) 
{
	bus->writeThenRead(sendBuffer, receiveBuffer, sendCount, receiveCount, deselectChip);
}

void SPIDevice::write(char* buffer, unsigned int count, bool deselectChip) 
{ 
	bus->write(buffer,count,deselectChip);
}

void SPIDevice::read(char* buffer, unsigned int count, bool deselectChip) 
{
	bus->read(buffer, count, deselectChip);
}