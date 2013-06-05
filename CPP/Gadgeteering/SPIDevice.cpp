#include "SPIDevice.hpp"
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

SPIDevice::SPIDevice(Socket* socket, Socket::Pin chipSelectPin, SPIDevice::Configuration* configuration) {
	socket->ensureTypeIsSupported(Socket::Types::S);

	this->chipSelect = new DigitalOutput(socket, chipSelectPin, true);
	this->configuration = configuration;
}

SPIDevice::~SPIDevice() {
	delete this->chipSelect;
}

char SPIDevice::writeReadByte(char toSend, bool deselectChip) { };

void SPIDevice::writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count, bool deselectChip) { };

void SPIDevice::writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, bool deselectChip) { };

void SPIDevice::write(char* buffer, unsigned int count, bool deselectChip) { };

void SPIDevice::read(char* buffer, unsigned int count, bool deselectChip) { };