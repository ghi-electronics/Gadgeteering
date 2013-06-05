#include "SPIBus.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

SPIBus::SPIBus(Socket* socket) {	
	this->socket = socket;
}

SPIBus::~SPIBus() {

}

char SPIBus::writeReadByte(char toSend, GHI::Interfaces::SPIDevice::Configuration* configuration) {
	mainboard->panic("No mainboard");

	return NULL;
}

void SPIBus::writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration) {

}

void SPIBus::writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, GHI::Interfaces::SPIDevice::Configuration* configuration) {

}

void SPIBus::write(char* buffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration) {

}

void SPIBus::read(char* buffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration) {

}