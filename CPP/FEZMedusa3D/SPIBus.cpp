/*
Copyright 2013 GHI Electronics LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "../Gadgeteering/Gadgeteering.h"
#include <SPI.h>

#include "FEZMedusa3D.h"
#include "Environment.h"

//#define SYSTEM_CLOCK 84000U /*KHz*/
//#define GADGETEERING_SYSTEM_CLOCK 84000U /*KHz*/
//#define GADGETEERING_EXTENDED_SPI

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;

FEZAthena::SPIBus::SPIBus(CPUPin mosi, CPUPin miso, CPUPin sck) : Interfaces::SPIBus(mosi, miso, sck)
{
#ifndef GADGETEERING_EXTENDED_SPI
	this->spi = new SPIClass();
	this->spi->begin();
#else
	SPI.begin(0);
#endif
}

FEZAthena::SPIBus::~SPIBus() {
#ifndef GADGETEERING_EXTENDED_SPI
	this->spi->end();
	delete this->spi;
#else
	SPI.end();
#endif
}

void FEZAthena::SPIBus::setup(GHI::Interfaces::SPIConfiguration* configuration) {
#ifndef GADGETEERING_EXTENDED_SPI
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
	
	switch (count) 
	{
		case 1: mainboard->panic(Exceptions::ERR_SPI_NOT_SUPPORTED); break;
		case 2: this->spi->setClockDivider(SPI_CLOCK_DIV2); break;
		case 3: this->spi->setClockDivider(SPI_CLOCK_DIV4); break;
		case 4: this->spi->setClockDivider(SPI_CLOCK_DIV8); break;
		case 5: this->spi->setClockDivider(SPI_CLOCK_DIV16); break;
		case 6: this->spi->setClockDivider(SPI_CLOCK_DIV32); break;
		case 7: this->spi->setClockDivider(SPI_CLOCK_DIV64); break;
		case 8: this->spi->setClockDivider(SPI_CLOCK_DIV128); break;
	}
#else

	if (!configuration->clockIdleState && configuration->clockEdge)
		SPI.setDataMode(configuration->chipSelect, SPI_MODE0);
	else if (!configuration->clockIdleState && !configuration->clockEdge)
		SPI.setDataMode(configuration->chipSelect, SPI_MODE1);
	else if (configuration->clockIdleState && !configuration->clockEdge)
		SPI.setDataMode(configuration->chipSelect, SPI_MODE2);
	else if (configuration->clockIdleState && configuration->clockEdge)
		SPI.setDataMode(configuration->chipSelect, SPI_MODE3);

	unsigned int divider = GADGETEERING_SYSTEM_CLOCK / configuration->clockRate;
	SPI.setClockDivider(0, divider);
#endif
}

void FEZAthena::SPIBus::selectChip(GHI::Interfaces::SPIConfiguration* configuration)
{
	mainboard->setIOMode(configuration->chipSelect, IOStates::DIGITAL_OUTPUT, ResistorModes::FLOATING);
	mainboard->writeDigital(configuration->chipSelect, configuration->chipSelectActiveState);
	System::Sleep(configuration->chipSelectSetupTime);
}

void FEZAthena::SPIBus::deselectChip(GHI::Interfaces::SPIConfiguration* configuration)
{
	mainboard->setIOMode(configuration->chipSelect, IOStates::DIGITAL_OUTPUT, ResistorModes::FLOATING);
	System::Sleep(configuration->chipSelectHoldTime);
	mainboard->writeDigital(configuration->chipSelect, !configuration->chipSelectActiveState);
//#ifdef GADGETEERING_EXTENDED_SPI
//	SPI.end();
//#endif
}

void FEZAthena::SPIBus::writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, Interfaces::SPIConfiguration* configuration, bool deselectAfter)
{
	this->setup(configuration);
	this->selectChip(configuration);

#ifndef GADGETEERING_EXTENDED_SPI
	for (int i = 0; i < count; i++) {
		if (sendBuffer != NULL && receiveBuffer != NULL)
			receiveBuffer[i] = this->spi->transfer(sendBuffer[i]);
		else if (sendBuffer != NULL && receiveBuffer == NULL)
			this->spi->transfer(sendBuffer[i]);
		else if (sendBuffer == NULL && receiveBuffer != NULL)
			receiveBuffer[i] = this->spi->transfer(0);
	}
#else
	for (int i = 0; i < count; i++) 
	{
		if (sendBuffer != NULL && receiveBuffer != NULL)
			receiveBuffer[i] = SPI.transfer(0, sendBuffer[i]);
		else if (sendBuffer != NULL && receiveBuffer == NULL)
			SPI.transfer(0, sendBuffer[i]);
		else if (sendBuffer == NULL && receiveBuffer != NULL)
			receiveBuffer[i] = SPI.transfer(0, 0);
	}
#endif
	
	if(deselectAfter)
		this->deselectChip(configuration);
}