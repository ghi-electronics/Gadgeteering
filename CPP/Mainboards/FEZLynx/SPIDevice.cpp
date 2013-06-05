#include <Gadgeteering/SPIDevice.hpp>

using namespace GHI;
using namespace GHI::Interfaces;

SPIDevice::~SPIDevice() {

}

byte SPIDevice::writeReadByte(byte toSend, bool deselectChip) {
	return this->bus->writeReadByte(toSend, deselectChip);
}

void SPIDevice::writeAndRead(byte* sendBuffer, byte* receiveBuffer, unsigned int count, bool deselectChip) {
	this->chipSelect->write(this->configuration->chipSelectActiveState);
	
	System::Sleep(this->configuration->chipSelectSetupTime);

	for (int i = 0; i < count; i++) {
		if (sendBuffer != NULL && receiveBuffer != NULL)
			receiveBuffer[i] = SPI.transfer(sendBuffer[i]);
		else if (sendBuffer != NULL && receiveBuffer == NULL)
			SPI.transfer(sendBuffer[i]);
		else if (sendBuffer == NULL && receiveBuffer != NULL)
			receiveBuffer[i] = SPI.transfer(0);
	}
	
	System::Sleep(this->configuration->chipSelectHoldTime);

	if (deselectChip)
		this->chipSelect->write(!this->configuration->chipSelectActiveState);
}

void SPIDevice::writeThenRead(byte* sendBuffer, byte* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, bool deselectChip) {
	this->write(sendBuffer, sendCount, deselectChip);
	this->write(receiveBuffer, receiveCount, deselectChip);
}

void SPIDevice::write(byte* buffer, unsigned int count, bool deselectChip) {
	this->writeAndRead(buffer, NULL, count, deselectChip);
}

void SPIDevice::read(byte* buffer, unsigned int count, bool deselectChip) {
	this->writeAndRead(NULL, buffer, count, deselectChip);
}