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

#include "FEZLynxS4.h"

using namespace GHI;
using namespace GHI::Mainboards;

FEZLynxS4::SPIBus::SPIBus(CPUPin mosiPin, CPUPin misoPin, CPUPin sckPin, FTDI_Device *device) : GHI::Interfaces::SPIBus(mosiPin, misoPin, sckPin)
{
    this->m_device = device;
}

FEZLynxS4::SPIBus::~SPIBus()
{

}

void FEZLynxS4::SPIBus::writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, GHI::Interfaces::SPIConfiguration* configuration, bool deselectAfter)
{
	unsigned char* buffer = new unsigned char[count + 3];
    DWORD sent = 0, read = 0, available = 0;
    FT_STATUS status;

    mainboard->setIOMode(configuration->chipSelect, IOStates::DIGITAL_OUTPUT);
    mainboard->writeDigital(configuration->chipSelect, !configuration->chipSelectActiveState);

    mainboard->setIOMode(this->miso, IOStates::DIGITAL_INPUT);
    mainboard->readDigital(this->miso);
    mainboard->setIOMode(this->mosi, IOStates::DIGITAL_OUTPUT);
    mainboard->writeDigital(this->mosi, false);
    mainboard->setIOMode(this->sck, IOStates::DIGITAL_OUTPUT);
    mainboard->writeDigital(this->sck, configuration->clockIdleState);

    mainboard->writeDigital(configuration->chipSelect, configuration->chipSelectActiveState);

    this->m_device->Open();

    this->m_device->Purge();

    unsigned short transfer_clock_divisor = (((60/(configuration->clockRate / 1000))/2)-1);

    buffer[0] = 0x86; //Set clock divisor
    buffer[1] = transfer_clock_divisor & 0xFF;
    buffer[2] = (transfer_clock_divisor >> 8) & 0xFF;
    status = FT_Write(this->m_device->GetHandle(), buffer, 3, &sent);

    if(status != FT_OK)
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR, status);

    buffer[0] = configuration->clockEdge ? 0x31 : 0x34;
    buffer[1] = (count - 1) & 0xFF;
    buffer[2] = ((count - 1) >> 8) & 0xFF;

    for (int i = 0; i < count; i++)
        buffer[i + 3] = sendBuffer != NULL ? sendBuffer[i] : 0x00;

	status |= FT_Write(this->m_device->GetHandle(), buffer, count + 3, &sent);

	if (receiveBuffer != NULL)
	{
        int timeout = 0;

        do {
            status |= FT_GetQueueStatus(this->m_device->GetHandle(), &available);
            System::Sleep(1);
            timeout++;
        } while (available < count && timeout < 2000);

		status |= FT_Read(this->m_device->GetHandle(), receiveBuffer, count, &read);
	}
	else {
		this->m_device->Purge();
		read = count;
	}

    //this->m_device->Close();

	delete [] buffer;

	if (deselectAfter)
        mainboard->writeDigital(configuration->chipSelect, !configuration->chipSelectActiveState);

	if(read != count || status != FT_OK)
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR, read);
}
