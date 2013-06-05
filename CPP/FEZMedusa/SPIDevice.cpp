#include <SPI.h>
#include "Arduino.h"
#include "../Gadgeteering/SPIDevice.hpp"
#include "../Gadgeteering/System.hpp"
#include "../Gadgeteering/Interfaces.hpp"
#include "FEZMedusa.h"

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;

#define SYSTEM_CLOCK 12000 //in KHz

FEZMedusa::SPIDevice::SPIDevice(Socket* socket, Socket::Pin chipSelectPin, SPIDevice::Configuration* configuration) : GHI::Interfaces::SPIDevice(socket, chipSelectPin, configuration) {
	this->spi = new SPIClass();
	this->spi->begin();
	
	if (!configuration->clockIdleState && configuration->clockEdge)
		this->spi->setDataMode(SPI_MODE0);
	else if (!configuration->clockIdleState && !configuration->clockEdge)
		this->spi->setDataMode(SPI_MODE1);
	else if (configuration->clockIdleState && !configuration->clockEdge)
		this->spi->setDataMode(SPI_MODE2);
	else if (configuration->clockIdleState && configuration->clockEdge)
		this->spi->setDataMode(SPI_MODE3);
	
	int divider = SYSTEM_CLOCK / configuration->clockRate;
	int count = 1;
	while ((divider >>= 1) > 0)
		count++;
	
	switch (count) {
		case 1: mainboard->panic("Speed not supported."); break;
		case 2: this->spi->setClockDivider(SPI_CLOCK_DIV2); break;
		case 3: this->spi->setClockDivider(SPI_CLOCK_DIV4); break;
		case 4: this->spi->setClockDivider(SPI_CLOCK_DIV8); break;
		case 5: this->spi->setClockDivider(SPI_CLOCK_DIV16); break;
		case 6: this->spi->setClockDivider(SPI_CLOCK_DIV32); break;
		case 7: this->spi->setClockDivider(SPI_CLOCK_DIV64); break;
		case 8: this->spi->setClockDivider(SPI_CLOCK_DIV128); break;
	}
}

FEZMedusa::SPIDevice::~SPIDevice() {
	this->spi->end();
}

char FEZMedusa::SPIDevice::writeReadByte(char toSend, bool deselectChip) {
	this->chipSelect->write(this->configuration->chipSelectActiveState);
	
	System::Sleep(this->configuration->chipSelectSetupTime);
	
	char result = this->spi->transfer(toSend);
	
	System::Sleep(this->configuration->chipSelectHoldTime);
	
	if (deselectChip)
		this->chipSelect->write(!this->configuration->chipSelectActiveState);
	
	return result;
}

void FEZMedusa::SPIDevice::writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count, bool deselectChip) {
	this->chipSelect->write(this->configuration->chipSelectActiveState);
	
	System::Sleep(this->configuration->chipSelectSetupTime);
	
	for (int i = 0; i < count; i++) {
		if (sendBuffer != NULL && receiveBuffer != NULL)
			receiveBuffer[i] = this->spi->transfer(sendBuffer[i]);
		else if (sendBuffer != NULL && receiveBuffer == NULL)
			this->spi->transfer(sendBuffer[i]);
		else if (sendBuffer == NULL && receiveBuffer != NULL)
			receiveBuffer[i] = this->spi->transfer(0);
	}
	
	System::Sleep(this->configuration->chipSelectHoldTime);
	
	if (deselectChip)
		this->chipSelect->write(!this->configuration->chipSelectActiveState);
}

void FEZMedusa::SPIDevice::writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, bool deselectChip) {
	this->write(sendBuffer, sendCount, deselectChip);
	this->write(receiveBuffer, receiveCount, deselectChip);
}

void FEZMedusa::SPIDevice::write(char* buffer, unsigned int count, bool deselectChip) {
	this->writeAndRead(buffer, NULL, count, deselectChip);
}

void FEZMedusa::SPIDevice::read(char* buffer, unsigned int count, bool deselectChip) {
	this->writeAndRead(NULL, buffer, count, deselectChip);
}
