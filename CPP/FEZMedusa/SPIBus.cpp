#include "../Gadgeteering/Gadgeteering.h"

#include "FEZMedusa.h"

using namespace GHI::Mainboards;

FEZMedusa::SPIBus::SPIBus(Socket* socket, FEZMedusa::SPIBus::Configuration config) : GHI::Interfaces::SPIBus(socket, config)
{
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

FEZMedusa::SPIBus::~SPIBus() {
	this->spi->end();
}

char FEZMedusa::SPIBus::writeReadByte(char toSend)
{
	System::Sleep(this->configuration->chipSelectSetupTime);
	
	char result = this->spi->transfer(toSend);
	
	System::Sleep(this->configuration->chipSelectHoldTime);
	
	return result;
}

void FEZMedusa::SPIBus::writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count)
{
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
}

void FEZMedusa::SPIBus::writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount)
{
	this->write(sendBuffer, sendCount);
	this->write(receiveBuffer, receiveCount);
}

void FEZMedusa::SPIBus::write(char* buffer, unsigned int count)
{	this->writeAndRead(buffer, NULL, count);

}

void FEZMedusa::SPIBus::read(char* buffer, unsigned int count)
{
	this->writeAndRead(NULL, buffer, count);
}