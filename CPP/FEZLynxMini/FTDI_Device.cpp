#include "include/FTDI_Device.h"

#include <iostream>

using namespace GHI;

FTDI_Configuration::FTDI_Configuration()
{
    this->BitMode = BIT_MODE_RESET;
    this->BaudRate = 115200;
    this->Timeout = 2000;
    this->Latency = 2;
}

FTDI_Device::FTDI_Device(unsigned char direction, unsigned char value, bool MPSSE, int index)
{
    this->m_direction = direction;
    this->m_value = value;
    this->m_mpsse = MPSSE;
    this->m_index = index;

    this->m_open = false;
    this->m_locked = false;
}

FTDI_Device::~FTDI_Device()
{
    this->Close();
}

void FTDI_Device::Open()
{
    if(this->m_open)
        return;

    while(this->m_locked)
        System::Sleep(2);
    this->m_locked = true;

    FT_STATUS status = FT_Open(this->m_index, &this->m_ftHandle);

    if (status != FT_OK)
        throw Exceptions::ERR_MAINBOARD_ERROR;

    this->m_open = true;
    this->m_locked = false;
}

void FTDI_Device::Close()
{
    while(this->m_locked)
        System::Sleep(2);

    if(!this->m_open)
        return;

    this->m_locked = true;

    FT_STATUS status = FT_Close(this->m_ftHandle);

    if (status != FT_OK)
        throw Exceptions::ERR_MAINBOARD_ERROR;

    this->m_open = false;
    this->m_locked = false;
}

void FTDI_Device::SetPinState()
{
    while(this->m_locked)
        System::Sleep(2);

    this->m_locked = true;

    DWORD sent = 0;
    FT_STATUS status = FT_OK;

	if (this->m_mpsse)
	{
		BYTE buffer[3] = { 0x80, this->m_value, this->m_direction };
		status |= FT_Write(this->m_ftHandle, buffer, 3, &sent);
	}
	else
	{
		status |= FT_SetBitMode(this->m_ftHandle, this->m_direction, 0x01);
		status |= FT_Write(this->m_ftHandle, &this->m_value, 1, &sent);
	}

    if(status != FT_OK)
        throw Exceptions::ERR_MAINBOARD_ERROR;

    this->m_locked = false;
}

unsigned char FTDI_Device::GetValue()
{
    while(this->m_locked)
        System::Sleep(2);

    this->m_locked = true;

    DWORD sent = 0;
	FT_STATUS status = FT_OK;

    if(this->m_mpsse)
    {
        DWORD dwNumInputBuffer = 0, dwNumBytesRead = 0;
        char *InputBuffer = new char[1024];

        do
        {
            status |= FT_GetQueueStatus(this->m_ftHandle, &dwNumInputBuffer); // Get the number of bytes in the FT2232H receive buffer

            if(dwNumInputBuffer == 0)
                break;

            if (dwNumInputBuffer <= 1024)
                status |= FT_Read(this->m_ftHandle, InputBuffer, dwNumInputBuffer, &dwNumBytesRead); //Read out the data from FT2232H receive buffer
            else
                status |= FT_Read(this->m_ftHandle, InputBuffer, 1024, &dwNumBytesRead);

        } while(dwNumInputBuffer > 0);

        delete [] InputBuffer;
    }
    else
    {
        FT_Purge(this->m_ftHandle, FT_PURGE_RX);
    }

	if(this->m_mpsse)
	{
		BYTE command = 0x81;
		status |= FT_Write(this->m_ftHandle, &command, 1, &sent);
	}

	status |= FT_Read(this->m_ftHandle, &this->m_value, 1, &sent);

	if (status != FT_OK)
        throw Exceptions::ERR_MAINBOARD_ERROR;

    this->m_locked = false;
    return this->m_value;
}

void FTDI_Device::Purge()
{
    while(this->m_locked)
        System::Sleep(2);

    this->m_locked = true;

    if(this->m_mpsse)
    {
        DWORD dwNumInputBuffer = 0, dwNumBytesRead = 0;
        char *InputBuffer = new char[1024];
        FT_STATUS ftStatus = FT_OK;

        do
        {
            ftStatus |= FT_GetQueueStatus(this->m_ftHandle, &dwNumInputBuffer); // Get the number of bytes in the FT2232H receive buffer

            if(dwNumInputBuffer == 0)
                break;

            if (dwNumInputBuffer <= 1024)
                ftStatus |= FT_Read(this->m_ftHandle, InputBuffer, dwNumInputBuffer, &dwNumBytesRead); //Read out the data from FT2232H receive buffer
            else
                ftStatus |= FT_Read(this->m_ftHandle, InputBuffer, 1024, &dwNumBytesRead);

        } while(dwNumInputBuffer > 0);

        delete [] InputBuffer;

        this->m_locked = false;

        return;
    }

    FT_Purge(this->m_ftHandle, FT_PURGE_RX);

    this->m_locked = false;
}

void FTDI_Device::BadCommandSync()
{
    if(!this->m_mpsse)
		return;

    while(this->m_locked)
        System::Sleep(2);

    this->Open();
    this->m_locked = true;

	DWORD read, sent;
	FT_STATUS status = FT_OK;
	BYTE buffer[5];

	buffer[0] = 0xAA;
    status = FT_Write(this->m_ftHandle, buffer, 1, &sent);

	buffer[0] = 0x00;
	buffer[1] = 0x00;
	buffer[2] = 0x00;

	unsigned char loops = 0x00;

    while (true)
	{
        status = FT_Read(this->m_ftHandle, buffer, 2, &read);

		if (buffer[0] == 0xFA && buffer[1] == 0xAA)
			break;

        if (++loops > 20)
            throw Exceptions::ERR_OUT_OF_SYNC;

        System::Sleep(10);
	}

	buffer[0] = 0x8A; //Disable clock divide by 5 for 60Mhz master clock
	buffer[1] = 0x97; //Turn off adaptive clocking
	//buffer[2] = 0x8D; //Enable 3 phase data clock
    status = FT_Write(this->m_ftHandle, buffer, 2, &sent);

	buffer[0] = 0x86; //Set clock divisor
    buffer[1] = 0x0055 & 0xFF;
    buffer[2] = (0x0055 >> 8) & 0xFF;
    status = FT_Write(this->m_ftHandle, buffer, 3, &sent);

	System::Sleep(20);

	buffer[0] = 0x85; //Disable loop back of TDI/TDO connection
    status = FT_Write(this->m_ftHandle, buffer, 1, &sent);

	System::Sleep(30);

	if (status != FT_OK)
        throw Exceptions::ERR_OUT_OF_SYNC;

    this->m_locked = false;
    this->Close();
}

void FTDI_Device::ClearDirection(unsigned char direction)
{
    while(this->m_locked)
        System::Sleep(2);

    this->m_locked = true;
    this->m_direction &= (~(1 << (direction - 1)));
    this->m_locked = false;
}

void FTDI_Device::SetDirection(unsigned char direction)
{
    while(this->m_locked)
        System::Sleep(2);

    this->m_locked = true;
    this->m_direction |= (1 << (direction - 1));
    this->m_locked = false;
}

void FTDI_Device::ClearValue(unsigned char value)
{
    while(this->m_locked)
        System::Sleep(2);

    this->m_locked = true;
    this->m_value &= (~(1 << (value - 1)));
    this->m_locked = false;
}

void FTDI_Device::SetValue(unsigned char value)
{
    while(this->m_locked)
        System::Sleep(2);

    this->m_locked = true;
    this->m_value |= (1 << (value - 1));
    this->m_locked = false;
}

unsigned char FTDI_Device::GetDirection()
{
    while(this->m_locked)
        System::Sleep(2);

    return this->m_direction;
}

FT_HANDLE FTDI_Device::GetHandle()
{
    while(this->m_locked)
        System::Sleep(2);

    return this->m_ftHandle;
}

void FTDI_Device::SetBaudRate(unsigned int baud)
{
    FT_STATUS status = FT_OK;

    while(this->m_locked)
        System::Sleep(2);

    this->m_locked = true;

    status |= FT_SetBaudRate(this->m_ftHandle, baud);

    this->m_locked = false;

    if(status != FT_OK)
        throw Exceptions::ERR_MAINBOARD_ERROR;
}

void FTDI_Device::SetBitMode(unsigned char mode)
{
    FT_STATUS status = FT_OK;

    while(this->m_locked)
        System::Sleep(2);

    this->m_locked = true;

    status |= FT_SetBitMode(this->m_ftHandle, this->m_direction, mode);

    this->m_locked = false;

    if(status != FT_OK)
        throw Exceptions::ERR_MAINBOARD_ERROR;
}

void FTDI_Device::SetLatency(unsigned int latency)
{
    FT_STATUS status = FT_OK;

    while(this->m_locked)
        System::Sleep(2);

    this->m_locked = true;

    status |= FT_SetLatencyTimer(this->m_ftHandle, latency);

    this->m_locked = false;

    if(status != FT_OK)
        throw Exceptions::ERR_MAINBOARD_ERROR;
}

void FTDI_Device::Reset()
{
    while(this->m_locked)
        System::Sleep(2);

    this->Open();
    this->m_locked = true;

    FT_STATUS ftStatus = FT_ResetDevice(this->m_ftHandle);
    System::Sleep(20);

    if(this->m_mpsse)
    {
        DWORD dwNumInputBuffer = 0, dwNumBytesRead = 0;
        char *InputBuffer = new char[1024];

        do
        {
            ftStatus |= FT_GetQueueStatus(this->m_ftHandle, &dwNumInputBuffer); // Get the number of bytes in the FT2232H receive buffer

            if(dwNumInputBuffer == 0)
                break;

            if (dwNumInputBuffer <= 1024)
                ftStatus |= FT_Read(this->m_ftHandle, InputBuffer, dwNumInputBuffer, &dwNumBytesRead); //Read out the data from FT2232H receive buffer
            else
                ftStatus |= FT_Read(this->m_ftHandle, InputBuffer, 1024, &dwNumBytesRead);

        } while(dwNumInputBuffer > 0);

        delete [] InputBuffer;

        this->m_locked = false;
        this->Close();

        return;
    }

    FT_Purge(this->m_ftHandle, FT_PURGE_RX);

    this->m_locked = false;
    this->Close();
}

void FTDI_Device::Pause()
{
    while(this->m_locked)
        System::Sleep(2);

    this->Close();
    this->m_locked = true;
}

void FTDI_Device::Resume()
{
    this->m_locked = false;
    this->Open();
}

void FTDI_Device::SetupChannel(FTDI_Configuration configuration)
{
    FT_STATUS status = FT_OK;

    while(this->m_locked)
        System::Sleep(2);

    this->Open();
    this->m_locked = true;

    status |= FT_ResetDevice(this->m_ftHandle);
    std::cout << " \\--Purging channel..." << std::endl;

    if(this->m_mpsse)
    {
        DWORD dwNumInputBuffer = 0, dwNumBytesRead = 0;
        char *InputBuffer = new char[1024];

        do
        {
            status |= FT_GetQueueStatus(this->m_ftHandle, &dwNumInputBuffer); // Get the number of bytes in the FT2232H receive buffer

            if(dwNumInputBuffer == 0)
                break;

            if (dwNumInputBuffer <= 1024)
                status |= FT_Read(this->m_ftHandle, InputBuffer, dwNumInputBuffer, &dwNumBytesRead); //Read out the data from FT2232H receive buffer
            else
                status |= FT_Read(this->m_ftHandle, InputBuffer, 1024, &dwNumBytesRead);

        } while(dwNumInputBuffer > 0);

        delete [] InputBuffer;

        this->m_locked = false;
    }
    else
    {
        FT_Purge(this->m_ftHandle, FT_PURGE_RX);
    }

    std::cout << "  |-Setting channel configuration..." << std::endl;

    status |= FT_SetUSBParameters(this->m_ftHandle, 65536, 65535);
    status |= FT_SetChars(this->m_ftHandle, false, 0, false, 0);
    status |= FT_SetTimeouts(this->m_ftHandle, configuration.Timeout, configuration.Timeout);
    status |= FT_SetLatencyTimer(this->m_ftHandle, configuration.Latency);
    status |= FT_SetBitMode(this->m_ftHandle, this->m_direction, 0x00);

    System::Sleep(50);

    status |= FT_SetBitMode(this->m_ftHandle, this->m_direction, configuration.BitMode);
    status |= FT_SetBaudRate(this->m_ftHandle, configuration.BaudRate);

    if(status != FT_OK)
        throw Exceptions::ERR_MAINBOARD_ERROR;

    System::Sleep(20);
    std::cout << "  |-Setting channel pin states..." << std::endl;

    DWORD sent = 0;

    if (this->m_mpsse)
    {
        BYTE buffer[3] = { 0x80, this->m_value, this->m_direction };
        status |= FT_Write(this->m_ftHandle, buffer, 3, &sent);
    }
    else
    {
        status |= FT_SetBitMode(this->m_ftHandle, this->m_direction, 0x01);
        status |= FT_Write(this->m_ftHandle, &this->m_value, 1, &sent);
    }

	if (status != FT_OK)
        throw Exceptions::ERR_OUT_OF_SYNC;

	System::Sleep(50);

    std::cout << " /--Complete." << std::endl;

	this->m_locked = false;
	this->Close();
}
