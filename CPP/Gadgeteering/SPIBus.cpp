#include "SPIBus.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

SPIBus::Configuration::Configuration(bool chipSelectActiveState, unsigned int chipSelectSetupTime, unsigned int chipSelectHoldTime, bool clockIdleState, bool clockEdge, unsigned int clockRate) {
	this->chipSelectActiveState = chipSelectActiveState;
	this->chipSelectSetupTime = chipSelectSetupTime;
	this->chipSelectHoldTime = chipSelectHoldTime;
	this->clockIdleState = clockIdleState;
	this->clockEdge = clockEdge;
	this->clockRate = clockRate;
}

SPIBus::SPIBus(Socket* socket, GHI::Interfaces::SPIBus::Configuration* configuration) {
	this->configuration = configuration;
	this->socket = socket;
}

SPIBus::~SPIBus() {
	delete this->configuration;
}

char writeReadByte(char toSend, bool deselectChip) {

}

void writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count, bool deselectChip) {

}

void writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, bool deselectChip) {

}

void write(char* buffer, unsigned int count, bool deselectChip) {

}

void read(char* buffer, unsigned int count, bool deselectChip) {

}