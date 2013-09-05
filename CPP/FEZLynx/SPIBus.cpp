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
	
	mainboard->setIOMode(this->miso, IOStates::DIGITAL_INPUT, ResistorModes::PULL_DOWN);
	mainboard->readDigital(this->sck);
	mainboard->setIOMode(this->sck, IOStates::DIGITAL_OUTPUT);
	mainboard->writeDigital(this->sck, true);
}

FEZLynx::SPIBus::~SPIBus()
{

}

void FEZLynx::SPIBus::writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, GHI::Interfaces::SPIConfiguration* configuration) 
{
    unsigned char next, activeState = !configuration->clockEdge ? 0x33 : 0x36;
	unsigned char buffer[4];
	DWORD sent, read, available, timeout = 0;
	FT_STATUS status;

	for (unsigned int i = 0; i < count; i++) {
		mainboard->writeDigital(this->sck, configuration->clockEdge);
		
		buffer[0] = activeState;
		buffer[1] = 0;
		buffer[2] = 0;
		buffer[3] = sendBuffer != NULL ? sendBuffer[i] : 0x00;

		status = FT_Write(channel, buffer, 4, &sent);
		
		do {
			status |= FT_GetQueueStatus(this->channel, &available);
			System::Sleep(1);
			timeout++;
		} while (available < count && timeout < 500);


		if(timeout >= 499 || status != FT_OK)
			mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);

		status = FT_Read(channel, &next, 1, &read);
		
		if (receiveBuffer != NULL)
			receiveBuffer[i] = next;

		if(read != 1 || status != FT_OK)
			mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);
	}
}
