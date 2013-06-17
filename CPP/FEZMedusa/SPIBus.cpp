#include "../Gadgeteering/Gadgeteering.h"
#include <SPI.h>

#include "FEZMedusa.h"

#define SYSTEM_CLOCK 12000U /*KHz*/

using namespace GHI;
using namespace GHI::Mainboards;

FEZMedusa::SPIBus::SPIBus(CPUPin mosi, CPUPin miso, CPUPin sck) : Interfaces::SPIBus(mosi, miso, sck)
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

	unsigned int divider = SYSTEM_CLOCK / configuration->clockRate;
	unsigned char count = 1;
	while ((divider >>= 1) > 0)
		count++;
	
	switch (count) {
		case 1: mainboard->panic(Exceptions::ERR_SPI_NOT_SUPPORTED); break;
		case 2: this->spi->setClockDivider(SPI_CLOCK_DIV2); break;
		case 3: this->spi->setClockDivider(SPI_CLOCK_DIV4); break;
		case 4: this->spi->setClockDivider(SPI_CLOCK_DIV8); break;
		case 5: this->spi->setClockDivider(SPI_CLOCK_DIV16); break;
		case 6: this->spi->setClockDivider(SPI_CLOCK_DIV32); break;
		case 7: this->spi->setClockDivider(SPI_CLOCK_DIV64); break;
		case 8: this->spi->setClockDivider(SPI_CLOCK_DIV128); break;
	}
}

void FEZMedusa::SPIBus::writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, Interfaces::SPIDevice::Configuration* configuration)
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