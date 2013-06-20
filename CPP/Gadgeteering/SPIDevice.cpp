#include "SPIDevice.hpp"
#include "SPIBus.hpp"
#include "Interfaces.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

SPIConfiguration::SPIConfiguration(bool chipSelectActiveState, unsigned int chipSelectSetupTime, unsigned int chipSelectHoldTime, bool clockIdleState, bool clockEdge, unsigned int clockRate) {
	this->chipSelectActiveState = chipSelectActiveState;
	this->chipSelectSetupTime = chipSelectSetupTime;
	this->chipSelectHoldTime = chipSelectHoldTime;
	this->clockIdleState = clockIdleState;
	this->clockEdge = clockEdge;
	this->clockRate = clockRate;
}

SPIDevice::SPIDevice(SPIBus *spiBus, CPUPin chipSelectPin, GHI::Interfaces::SPIConfiguration* configuration) {
	this->chipSelect = new DigitalOutput(chipSelectPin, true);
	this->configuration = configuration;

	this->bus = spiBus;
}

SPIDevice::SPIDevice(SPIBus* spiBus, Socket* socket, Socket::Pin chipSelectPinNumber, GHI::Interfaces::SPIConfiguration* configuration) {
	socket->ensureTypeIsSupported(Socket::Types::S);

	this->chipSelect = new DigitalOutput(socket, chipSelectPinNumber, true);
	this->configuration = configuration;

	this->bus = spiBus;
}

SPIDevice::~SPIDevice() {
	delete this->configuration;
	delete this->chipSelect;
}

unsigned char SPIDevice::writeReadByte(unsigned char toSend, bool deselectChip) 
{ 
	this->chipSelect->write(this->configuration->chipSelectActiveState);

	unsigned char received;
	this->bus->writeRead(&toSend, &received, 1, this->configuration);

	if (deselectChip)
		this->chipSelect->write(!this->configuration->chipSelectActiveState);

	return received;
}

void SPIDevice::writeAndRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, bool deselectChip) 
{
	this->chipSelect->write(this->configuration->chipSelectActiveState);

	this->bus->writeRead(sendBuffer, receiveBuffer, count, this->configuration);

	if (deselectChip)
		this->chipSelect->write(!this->configuration->chipSelectActiveState);
}

void SPIDevice::writeThenRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, bool deselectChip) 
{
	this->chipSelect->write(this->configuration->chipSelectActiveState);
	
	this->bus->writeRead(sendBuffer, NULL, sendCount, configuration);
	this->bus->writeRead(NULL, receiveBuffer, receiveCount, configuration);

	if (deselectChip)
		this->chipSelect->write(!this->configuration->chipSelectActiveState);
}

void SPIDevice::write(const unsigned char* buffer, unsigned int count, bool deselectChip) 
{ 
	this->chipSelect->write(this->configuration->chipSelectActiveState);

	this->bus->writeRead(buffer, NULL, count, configuration);

	if (deselectChip)
		this->chipSelect->write(!this->configuration->chipSelectActiveState);
}

void SPIDevice::read(unsigned char* buffer, unsigned int count, bool deselectChip) 
{
	this->chipSelect->write(this->configuration->chipSelectActiveState);

	this->bus->writeRead(NULL, buffer, count, configuration);

	if (deselectChip)
		this->chipSelect->write(!this->configuration->chipSelectActiveState);
}