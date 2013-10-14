#include "FEZLynx.h"

#include "../Gadgeteering/System.hpp"

using namespace GHI;
using namespace GHI::Mainboards;

FEZLynx::SerialDevice::SerialDevice(CPUPin tx, CPUPin rx, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits, FTDI_Device *device) : Interfaces::SerialDevice(tx, rx, baudRate, parity, stopBits, dataBits)
{
    this->m_device = device;
}

FEZLynx::SerialDevice::~SerialDevice()
{

}

void FEZLynx::SerialDevice::open()
{
    this->m_device->SetBitMode(FTDI_Configuration::BIT_MODE_SERIAL);
    this->m_device->SetBaudRate(this->baudRate);
    this->m_device->Open();
}

void FEZLynx::SerialDevice::close()
{
    this->m_device->Close();
}

void FEZLynx::SerialDevice::write(const unsigned char* buffer, unsigned int count)
{
    DWORD sent = 0;

    if(FT_Write(this->m_device->GetHandle(), const_cast<unsigned char*>(buffer), count, &sent) != FT_OK || sent != count)
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);
}

unsigned int FEZLynx::SerialDevice::read(unsigned char* buffer, unsigned int count)
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

unsigned int FEZLynx::SerialDevice::available()
{
    DWORD available = 0;

	if(FT_GetQueueStatus(this->m_device->GetHandle(), &available) != FT_OK)
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);

	return available;
}
