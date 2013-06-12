#include "SPIBus.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

SPIBus::SPIBus(Socket* socket) {	
	this->socket = socket;
}

SPIBus::~SPIBus() {

}

char SPIBus::writeReadByte(unsigned char toSend, GHI::Interfaces::SPIDevice::Configuration* configuration) {
	mainboard->panic("No mainboard");

	return NULL;
}

void SPIBus::writeAndRead(unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration) {

}

void SPIBus::writeThenRead(unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, GHI::Interfaces::SPIDevice::Configuration* configuration) {

}

void SPIBus::write(unsigned char* buffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration) {

}

void SPIBus::read(unsigned char* buffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration) {

}