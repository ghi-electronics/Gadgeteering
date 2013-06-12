#include "../Gadgeteering/Gadgeteering.h"
#include <SPI.h>

#include "FEZMedusa.h"

#define SYSTEM_CLOCK 12000 /*KHz*/

using namespace GHI::Mainboards;

FEZMedusa::SPIBus::SPIBus(Socket* socket) : GHI::Interfaces::SPIBus(socket)
{
	this->spi = new SPIClass();
	this->spi->begin();
}

FEZMedusa::SPIBus::~SPIBus() {
	this->spi->end();
}

void FEZMedusa::SPIBus::setup(GHI::Interfaces::SPIDevice::Configuration* configuration) {
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

char FEZMedusa::SPIBus::writeReadByte(unsigned char toSend, GHI::Interfaces::SPIDevice::Configuration* configuration)
{
	this->setup(configuration);

	System::Sleep(configuration->chipSelectSetupTime);
	
	char result = this->spi->transfer(toSend);
	
	System::Sleep(configuration->chipSelectHoldTime);
	
	return result;
}

void FEZMedusa::SPIBus::writeAndRead(unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration)
{
	this->setup(configuration);

	System::Sleep(configuration->chipSelectSetupTime);
	
	for (int i = 0; i < count; i++) {
		if (sendBuffer != NULL && receiveBuffer != NULL)
			receiveBuffer[i] = this->spi->transfer(sendBuffer[i]);
		else if (sendBuffer != NULL && receiveBuffer == NULL)
			this->spi->transfer(sendBuffer[i]);
		else if (sendBuffer == NULL && receiveBuffer != NULL)
			receiveBuffer[i] = this->spi->transfer(0);
	}
	
	System::Sleep(configuration->chipSelectHoldTime);
}

void FEZMedusa::SPIBus::writeThenRead(unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, GHI::Interfaces::SPIDevice::Configuration* configuration)
{
	this->write(sendBuffer, sendCount, configuration);
	this->read(receiveBuffer, receiveCount, configuration);
}

void FEZMedusa::SPIBus::write(unsigned char* buffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration)
{	this->writeAndRead(buffer, NULL, count, configuration);

}

void FEZMedusa::SPIBus::read(unsigned char* buffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration)
{
	this->writeAndRead(NULL, buffer, count, configuration);
}