#include "FEZLynx.h"

#include "../Gadgeteering/System.hpp"

using namespace GHI;
using namespace GHI::Mainboards;

FEZLynx::SerialDevice::SerialDevice(CPUPin tx, CPUPin rx, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits, FT_HANDLE channel) : Interfaces::SerialDevice(tx, rx, baudRate, parity, stopBits, dataBits)
{

}

FEZLynx::SerialDevice::~SerialDevice()
{

}

void FEZLynx::SerialDevice::open()
{
    FT_SetBaudRate(this->channel, this->baudRate);
}

void FEZLynx::SerialDevice::close()
{

}

void FEZLynx::SerialDevice::write(const unsigned char* buffer, unsigned int count)
{
    DWORD sent = 0;

    if(FT_Write(this->channel, const_cast<unsigned char*>(buffer), count, &sent) != FT_OK || sent != count)
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);
}

unsigned int FEZLynx::SerialDevice::read(unsigned char* buffer, unsigned int count)
{
    DWORD available, read;
    int timeout = 0;
    FT_STATUS status = FT_OK;

	do {
        status |= FT_GetQueueStatus(this->channel, &available);
        System::Sleep(1);
        timeout++;
	} while (available < count && timeout < 500);


    if(timeout >= 499 || status != FT_OK)
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);

    status = FT_Read(this->channel, buffer, count, &read);

    if(read != count || status != FT_OK)
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);

    return read;
}

unsigned int FEZLynx::SerialDevice::available()
{
    DWORD available = 0;

	if(FT_GetQueueStatus(channel, &available) != FT_OK)
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);

	return available;
}
