#include "SPIDevice.hpp"
#include <SPI.h>

#define SYSTEM_CLOCK 16000 //in KHz

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
	SPI.begin();
	
	if (!configuration->clockIdleState && configuration->clockEdge)
		SPI.setDataMode(SPI_MODE0);
	else if (!configuration->clockIdleState && !configuration->clockEdge)
		SPI.setDataMode(SPI_MODE1);
	else if (configuration->clockIdleState && !configuration->clockEdge)
		SPI.setDataMode(SPI_MODE2);
	else if (configuration->clockIdleState && configuration->clockEdge)
		SPI.setDataMode(SPI_MODE3);
	
	int divider = SYSTEM_CLOCK / configuration->clockRate;
	int count = 1;
	while ((divider >>= 1) > 0)
		count++;

	switch (count) {
		case 1: mainboard->panic("Speed not supported."); break;
		case 2: SPI.setClockDivider(SPI_CLOCK_DIV2); break;
		case 3: SPI.setClockDivider(SPI_CLOCK_DIV4); break;
		case 4: SPI.setClockDivider(SPI_CLOCK_DIV8); break;
		case 5: SPI.setClockDivider(SPI_CLOCK_DIV16); break;
		case 6: SPI.setClockDivider(SPI_CLOCK_DIV32); break;
		case 7: SPI.setClockDivider(SPI_CLOCK_DIV64); break;
		case 8: SPI.setClockDivider(SPI_CLOCK_DIV128); break;
	}
}

SPIDevice::~SPIDevice() {
	SPI.end();
	delete this->chipSelect;
}

byte SPIDevice::writeReadByte(byte toSend, bool deselectChip) {
	this->chipSelect->write(this->configuration->chipSelectActiveState);

	delay(this->configuration->chipSelectSetupTime);

	byte result = SPI.transfer(toSend);

	delay(this->configuration->chipSelectHoldTime);

	if (deselectChip)
		this->chipSelect->write(!this->configuration->chipSelectActiveState);

	return result;
}

void SPIDevice::writeAndRead(byte* sendBuffer, byte* receiveBuffer, unsigned int count, bool deselectChip) {
	this->chipSelect->write(this->configuration->chipSelectActiveState);

	delay(this->configuration->chipSelectSetupTime);

	for (int i = 0; i < count; i++) {
		if (sendBuffer != NULL && receiveBuffer != NULL)
			receiveBuffer[i] = SPI.transfer(sendBuffer[i]);
		else if (sendBuffer != NULL && receiveBuffer == NULL)
			SPI.transfer(sendBuffer[i]);
		else if (sendBuffer == NULL && receiveBuffer != NULL)
			receiveBuffer[i] = SPI.transfer(0);
	}

	delay(this->configuration->chipSelectHoldTime);

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
