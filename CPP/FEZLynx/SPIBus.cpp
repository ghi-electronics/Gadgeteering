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

#include "FEZLynx.h"
#include "../Gadgeteering/System.hpp"
#include "../Gadgeteering/Types.hpp"

using namespace GHI;
using namespace GHI::Mainboards;

FEZLynx::SPIBus::SPIBus(CPUPin mosiPin, CPUPin misoPin, CPUPin sckPin, FT_HANDLE channel) : GHI::Interfaces::SPIBus(mosiPin, misoPin, sckPin)
{
	this->channel = channel;
}

FEZLynx::SPIBus::~SPIBus()
{

}

void FEZLynx::SPIBus::writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, CPUPin chipSelect, GHI::Interfaces::SPIConfiguration* configuration, bool deselectAfter) 
{
	unsigned char* buffer = new unsigned char[count + 3];
	DWORD sent, read = 0;
	FT_STATUS status;
	
	mainboard->setIOMode(chipSelect, IOStates::DIGITAL_OUTPUT);
	mainboard->writeDigital(chipSelect, !configuration->chipSelectActiveState);
	mainboard->setIOMode(this->miso, IOStates::DIGITAL_INPUT);
	mainboard->readDigital(this->miso);
	mainboard->setIOMode(this->mosi, IOStates::DIGITAL_OUTPUT);
	mainboard->writeDigital(this->mosi, false);
	mainboard->setIOMode(this->sck, IOStates::DIGITAL_OUTPUT);
	mainboard->writeDigital(this->sck, configuration->clockIdleState);

	mainboard->writeDigital(chipSelect, configuration->chipSelectActiveState);
	
	buffer[0] = configuration->clockEdge ? 0x31 : 0x34;
	buffer[1] = (count - 1) & 0xFF;
	buffer[2] = ((count - 1) >> 8) & 0xFF;

	for (int i = 0; i < count; i++)
		buffer[i + 3] = sendBuffer != NULL ? sendBuffer[i] : 0x00;
	
	FT_Purge(channel, FT_PURGE_TX);
	status = FT_Write(channel, buffer, count + 3, &sent);

	if (receiveBuffer != NULL) {
		status |= FT_Read(channel, receiveBuffer, count, &read);
	}
	else {
		FT_Purge(channel, FT_PURGE_RX);
		read = count;
	}

	delete [] buffer;

	if (deselectAfter)
		mainboard->writeDigital(chipSelect, !configuration->chipSelectActiveState);

	if(read != count || status != FT_OK)
		mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);
}
