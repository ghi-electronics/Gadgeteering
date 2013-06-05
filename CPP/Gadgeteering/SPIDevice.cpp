#include "SPIDevice.hpp"

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