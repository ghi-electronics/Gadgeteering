#include "SPIBus.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

SPIBus::SPIBus(Socket* socket) {	
	this->socket = socket;
}

SPIBus::~SPIBus() {

}

char writeReadByte(char toSend, bool deselectChip, GHI::Interfaces::SPIDevice::Configuration* configuration) {

}

void writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count, bool deselectChip, GHI::Interfaces::SPIDevice::Configuration* configuration) {

}

void writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, bool deselectChip, GHI::Interfaces::SPIDevice::Configuration* configuration) {

}

void write(char* buffer, unsigned int count, bool deselectChip, GHI::Interfaces::SPIDevice::Configuration* configuration) {

}

void read(char* buffer, unsigned int count, bool deselectChip, GHI::Interfaces::SPIDevice::Configuration* configuration) {

}