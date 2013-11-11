/*
Copyright 2013 Gadgeteering Electronics LLC

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

using namespace Gadgeteering;
using namespace Gadgeteering::Mainboards;

FEZLynxS4::SerialDevice::SerialDevice(CPUPin tx, CPUPin rx, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits, FTDI_Device *device) : Interfaces::SerialDevice(tx, rx, baudRate, parity, stopBits, dataBits)
{
    this->m_device = device;
}

FEZLynxS4::SerialDevice::~SerialDevice()
{

}

void FEZLynxS4::SerialDevice::open()
{
    this->m_device->SetBitMode(FTDI_Configuration::BIT_MODE_SERIAL);
    this->m_device->SetBaudRate(this->baudRate);
    this->m_device->Open();
}

void FEZLynxS4::SerialDevice::close()
{
    this->m_device->Close();
}

void FEZLynxS4::SerialDevice::write(const unsigned char* buffer, unsigned int count)
{
    DWORD sent = 0;

    if(FT_Write(this->m_device->GetHandle(), const_cast<unsigned char*>(buffer), count, &sent) != FT_OK || sent != count)
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);
}

unsigned int FEZLynxS4::SerialDevice::read(unsigned char* buffer, unsigned int count)
{
    DWORD available, read;
    int timeout = 0;
    FT_STATUS status = FT_OK;

	do {
        status |= FT_GetQueueStatus(this->m_device->GetHandle(), &available);
        System::Sleep(1);
        timeout++;
	} while (available < count && timeout < 2000);

    if(timeout >= 499 || status != FT_OK)
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);

    status = FT_Read(this->m_device->GetHandle(), buffer, count, &read);

    if(read != count || status != FT_OK)
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);

    return read;
}

unsigned int FEZLynxS4::SerialDevice::available()
{
    DWORD available = 0;

	if(FT_GetQueueStatus(this->m_device->GetHandle(), &available) != FT_OK)
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);

	return available;
}
