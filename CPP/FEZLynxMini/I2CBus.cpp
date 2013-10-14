#include "FEZLynx.h"

using namespace GHI;
using namespace Mainboards;

FEZLynx::I2CBus::I2CBus(CPUPin sdaPin, CPUPin sclPin, FTDI_Device *device) : Interfaces::I2CBus(sdaPin, sclPin)
{
    this->m_device = device;
}

FEZLynx::I2CBus::~I2CBus()
{
}

void FEZLynx::I2CBus::sendStopCondition()
{
    //Set SDA & SCL Output Low
    this->m_device->SetDirection(FTDI_PIN(this->scl));
    this->m_device->SetDirection(FTDI_PIN(this->sda));
    this->m_device->ClearValue(FTDI_PIN(this->scl));
    this->m_device->ClearValue(FTDI_PIN(this->sda));
    this->m_device->SetPinState();

    //Send Stop Condition
    this->m_device->SetValue(FTDI_PIN(this->scl));
    this->m_device->SetPinState();
    this->m_device->SetValue(FTDI_PIN(this->sda));
    this->m_device->SetPinState();
}

bool FEZLynx::I2CBus::sendStartCondition(unsigned char address)
{
    unsigned char* buffer = new unsigned char[20];
    DWORD sent = 0, read = 0, timeout = 0, dwNumBytesToSend = 0;
    FT_STATUS status;

//    //Set SDA & SCL Output High
//    this->m_device->SetDirection(FTDI_PIN(this->scl));
//    this->m_device->SetDirection(FTDI_PIN(this->sda));
//    this->m_device->SetValue(FTDI_PIN(this->scl));
//    this->m_device->SetValue(FTDI_PIN(this->sda));
//    this->m_device->SetPinState();
//
//    //Send Start Condition
//    this->m_device->ClearValue(FTDI_PIN(this->sda));
//    this->m_device->SetPinState();
//    this->m_device->ClearValue(FTDI_PIN(this->scl));
//    this->m_device->SetPinState();
////    this->m_device->SetValue(FTDI_PIN(this->scl));
////    this->m_device->SetPinState();
//
//    buffer[0] = 0x10;
//    buffer[1] = 0;
//    buffer[2] = 0;
//    buffer[3] = address;
//    status |= FT_Write(this->m_device->GetHandle(), buffer, 4, &sent);
//
//    //Set SDA & SCL To Input
//    this->m_device->ClearDirection(FTDI_PIN(this->scl));
//    this->m_device->ClearDirection(FTDI_PIN(this->sda));
//    this->m_device->SetPinState();

	DWORD dwCount;
	for(dwCount=0; dwCount<4; dwCount++) // Repeat commands to ensure the minimum period of the stop setup time ie 600ns is achieved
	{
		buffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
		buffer[dwNumBytesToSend++] = '\x01'; //Set SDA low, SCL high, WP disabled by SK at bit „1‟, DO, GPIOL0 at bit „0‟
		buffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit „1‟, other pins as input with bit „0‟
	}
	for(dwCount=0; dwCount<4; dwCount++) // Repeat commands to ensure the minimum period of the stop hold time ie 600ns is achieved
	{
		buffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
		buffer[dwNumBytesToSend++] = '\x03'; //Set SDA, SCL high, WP disabled by SK, DO at bit „1‟, GPIOL0 at bit „0‟
		buffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit „1‟, other pins as input with bit „0‟
	}

	//Tristate the SCL, SDA pins
	buffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
	buffer[dwNumBytesToSend++] = '\x00'; //Set WP disabled by GPIOL0 at bit „0‟
	buffer[dwNumBytesToSend++] = '\x10'; //Set GPIOL0 pins as output with bit „1‟, SK, DO and other pins as input with bit „0‟

	buffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
	buffer[dwNumBytesToSend++] = '\x01'; //Set SDA low, SCL high, WP disabled by SK at bit „1‟, DO, GPIOL0 at bit „0‟
	buffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit „1‟, other pins as input with bit „0‟

	status = FT_Write(this->m_device->GetHandle(), buffer, dwNumBytesToSend, &sent); //Send off the commands
	dwNumBytesToSend = 0; //Clear output buffer
	
	buffer[dwNumBytesToSend++] = '\x80';//Command to set directions of lower 8 pins and force value on bits set as output
	buffer[dwNumBytesToSend++] = '\x03'; //Set SDA, SCL high, WP disabled by SK, DO at bit „1‟, GPIOL0 at bit „0‟
	buffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit „1‟, other pins as input with bit „0‟

	status = FT_Write(this->m_device->GetHandle(), buffer, dwNumBytesToSend, &sent); //Send off the commands
	dwNumBytesToSend = 0; //Clear output buffer
	
	//Tristate the SCL, SDA pins
	buffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
	buffer[dwNumBytesToSend++] = '\x00'; //Set WP disabled by GPIOL0 at bit „0‟
	buffer[dwNumBytesToSend++] = '\x10'; //Set GPIOL0 pins as output with bit „1‟, SK, DO and other pins as input with bit „0‟

	status = FT_Write(this->m_device->GetHandle(), buffer, dwNumBytesToSend, &sent); //Send off the commands
	dwNumBytesToSend = 0; //Clear output buffer

    while(!(this->m_device->GetValue() & FTDI_PIN(this->scl)))
    {
        if(++timeout > 100)
            mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR, 255);
    }

    bool nack = (this->m_device->GetValue() & FTDI_PIN(this->sda));

    delete [] buffer;

    return !nack;
}

unsigned int FEZLynx::I2CBus::write(const unsigned char *buffer, unsigned int count, unsigned char address, bool sendStop)
{
    unsigned char* writeBuffer = new unsigned char[count + 3];
    DWORD sent = 0;
    FT_STATUS status;

    this->m_device->Open();
    this->sendStartCondition(address);

    writeBuffer[0] = 0x10;
    writeBuffer[1] = (count - 1) & 0xFF;
    writeBuffer[2] = ((count - 1) >> 8) & 0xFF;
    for (int i = 0; i < count; i++)
        writeBuffer[i + 3] = buffer != NULL ? buffer[i] : 0x00;

    status |= FT_Write(this->m_device->GetHandle(), writeBuffer, count + 3, &sent);

    if(sendStop)
    {
        this->sendStopCondition();
        this->m_device->Close();
    }

    if((sent != count) || (status != FT_OK))
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);

	return sent;
}

unsigned int FEZLynx::I2CBus::read(unsigned char *buffer, unsigned int count, unsigned char address, bool sendStop)
{
    DWORD read = 0, available = 0, sent = 0;
    unsigned char obuffer[3];

    FT_STATUS status = FT_OK;

    this->m_device->Open();
    this->m_device->Purge();

    this->sendStartCondition(address);

    obuffer[0] = 0x20;
    obuffer[1] = (count - 1) & 0xFF;
    obuffer[2] = ((count - 1) >> 8) & 0xFF;

    status |= FT_Write(this->m_device->GetHandle(), obuffer, 3, &sent);

    int timeout = 0;

    do {
        status |= FT_GetQueueStatus(this->m_device->GetHandle(), &available);
        System::Sleep(1);
        timeout++;
    } while (available < count && timeout < 2000);

    status |= FT_Read(this->m_device->GetHandle(), buffer, count, &read);

    if(sendStop)
    {
        this->sendStopCondition();
        this->m_device->Close();
    }

    if((read != count) || (status != FT_OK))
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR, 254);

	return read;
}

bool FEZLynx::I2CBus::writeRead(const unsigned char *writeBuffer, unsigned int writeLength, unsigned char *readBuffer, unsigned int readLength, unsigned int *numWritten, unsigned int *numRead, unsigned char address)
{
    unsigned char* buffer = new unsigned char[writeLength + 3];
    DWORD sent = 0, read = 0, available = 0;
    FT_STATUS status;

    this->m_device->Open();
    this->sendStartCondition(address);

    buffer[0] = 0x31;
    buffer[1] = (writeLength - 1) & 0xFF;
    buffer[2] = ((writeLength - 1) >> 8) & 0xFF;
    for (int i = 0; i < writeLength; i++)
        buffer[i + 3] = writeBuffer != NULL ? writeBuffer[i] : 0x00;

    status |= FT_Write(this->m_device->GetHandle(), buffer, writeLength + 3, &sent);

    this->sendStopCondition();

    if (readLength > 0)
    {
        //this->m_device->Purge();

        this->sendStartCondition(address | 1);

        buffer[0] = 0x20;
        buffer[1] = (readLength - 1) & 0xFF;
        buffer[2] = ((readLength - 1) >> 8) & 0xFF;

        status |= FT_Write(this->m_device->GetHandle(), buffer, 3, &sent);

        int timeout = 0;

        do {
            status |= FT_GetQueueStatus(this->m_device->GetHandle(), &available);
            System::Sleep(1);

            if(++timeout >= 2000)
                mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);

        } while (available < readLength);

        status |= FT_Read(this->m_device->GetHandle(), readBuffer, readLength, &read);

        this->sendStopCondition();
    }

    this->m_device->Close();

    delete [] buffer;

    if((read != readLength) || (status != FT_OK))
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);

	return true;
}
