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
using namespace Mainboards;

FEZLynxS4::I2CBus::I2CBus(CPUPin sdaPin, CPUPin sclPin, FTDI_Device *device) : Interfaces::I2CBus(sdaPin, sclPin)
{
    this->m_device = device;
}

FEZLynxS4::I2CBus::~I2CBus()
{
}

void FEZLynxS4::I2CBus::sendStopCondition()
{
    char* buffer = new char[30];
    DWORD sent = 0, read = 0, timeout = 0, dwNumBytesToSend = 0;
    FT_STATUS status;

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
	
	buffer[dwNumBytesToSend++] = '\x80';//Command to set directions of lower 8 pins and force value on bits set as output
	buffer[dwNumBytesToSend++] = '\x03'; //Set SDA, SCL high, WP disabled by SK, DO at bit „1‟, GPIOL0 at bit „0‟
	buffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit „1‟, other pins as input with bit „0‟
	
	//Tristate the SCL, SDA pins
	buffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
	buffer[dwNumBytesToSend++] = '\x00'; //Set WP disabled by GPIOL0 at bit „0‟
	buffer[dwNumBytesToSend++] = '\x10'; //Set GPIOL0 pins as output with bit „1‟, SK, DO and other pins as input with bit „0‟

	status = FT_Write(this->m_device->GetHandle(), buffer, dwNumBytesToSend, &sent); //Send off the commands
	dwNumBytesToSend = 0; //Clear output buffer

	//delete [] buffer;

	return;
}

bool FEZLynxS4::I2CBus::sendStartCondition(unsigned char address)
{
    char* buffer = new char[30];

    DWORD sent = 0, read = 0, timeout = 0, dwNumBytesToSend = 0;
    FT_STATUS status;

	DWORD dwCount;
	for(dwCount=0; dwCount < 4; dwCount++) // Repeat commands to ensure the minimum period of the start hold time ie 600ns is achieved
	{
		buffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
		buffer[dwNumBytesToSend++] = '\x03'; //Set SDA, SCL high, WP disabled by SK, DO at bit „1‟, GPIOL0 at bit „0‟
		buffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit „1‟, other pins as input with bit „0‟
	}
	for(dwCount=0; dwCount < 4; dwCount++) // Repeat commands to ensure the minimum period of the start setup time ie 600ns is achieved
	{
		buffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
		buffer[dwNumBytesToSend++] = '\x01'; //Set SDA low, SCL high, WP disabled by SK at bit „1‟, DO, GPIOL0 at bit „0‟
		buffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit „1‟, other pins as input with bit „0‟
	}
	
	buffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
	buffer[dwNumBytesToSend++] = '\x01'; //Set SDA low, SCL high, WP disabled by SK at bit „1‟, DO, GPIOL0 at bit „0‟
	buffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit „1‟, other pins as input with bit „0‟
	
	buffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
	buffer[dwNumBytesToSend++] = '\x00'; //Set SDA, SCL low, WP disabled by SK, DO, GPIOL0 at bit „0‟
	buffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit „1‟, other pins as input with bit „0‟
	
	buffer[dwNumBytesToSend++] = 0x11;
	buffer[dwNumBytesToSend++] = 0;
	buffer[dwNumBytesToSend++] = 0;
	buffer[dwNumBytesToSend++] = address;
	
	buffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
	buffer[dwNumBytesToSend++] = '\x00'; //Set SDA, SCL low, WP disabled by SK, DO, GPIOL0 at bit „0‟
	buffer[dwNumBytesToSend++] = '\x10'; //Set SK,DO,GPIOL0 pins as output with bit „1‟, other pins as input with bit „0‟

	status = FT_Write(this->m_device->GetHandle(), buffer, dwNumBytesToSend, &sent); //Send off the commands
	dwNumBytesToSend = 0; //Clear output buffer

    while(!(this->m_device->GetValue() & FTDI_PIN(this->scl)))
    {
        if(++timeout > 100)
            mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR, 255);
    }

    bool nack = (this->m_device->GetValue() & FTDI_PIN(this->sda));

    //delete [] buffer;

    return !nack;
}

unsigned int FEZLynxS4::I2CBus::write(const unsigned char *buffer, unsigned int count, unsigned char address, bool sendStop)
{
    unsigned char* writeBuffer = new unsigned char[count + 3];
    DWORD sent = 0;

    this->sendStartCondition(address);

    writeBuffer[0] = 0x11;
    writeBuffer[1] = (count - 1) & 0xFF;
    writeBuffer[2] = ((count - 1) >> 8) & 0xFF;
    for (int i = 0; i < count; i++)
        writeBuffer[i + 3] = buffer != NULL ? buffer[i] : 0x00;

	FT_STATUS status = FT_Write(this->m_device->GetHandle(), writeBuffer, count + 3, &sent);

    if(sendStop)
        this->sendStopCondition();

    if((sent != count + 3) || (status != FT_OK))
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);

	return sent;
}

unsigned int FEZLynxS4::I2CBus::read(unsigned char *buffer, unsigned int count, unsigned char address, bool sendStop)
{
    DWORD read = 0, available = 0, sent = 0;
    unsigned char obuffer[3];

    FT_STATUS status = FT_OK;

    this->m_device->Purge();

    this->sendStartCondition(address);

    obuffer[0] = 0x24;
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
        this->sendStopCondition();

    if((read != count) || (status != FT_OK))
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR, 254);

	return read;
}

bool FEZLynxS4::I2CBus::writeRead(const unsigned char *writeBuffer, unsigned int writeLength, unsigned char *readBuffer, unsigned int readLength, unsigned int *numWritten, unsigned int *numRead, unsigned char address)
{
    unsigned char* buffer = new unsigned char[writeLength + 3];
    DWORD sent = 0, read = 0, available = 0;
    FT_STATUS status = FT_OK;

    this->m_device->Open();
    this->sendStartCondition(address);

    buffer[0] = 0x11;
    buffer[1] = (writeLength - 1) & 0xFF;
    buffer[2] = ((writeLength - 1) >> 8) & 0xFF;
    for (int i = 0; i < writeLength; i++)
        buffer[i + 3] = writeBuffer != NULL ? writeBuffer[i] : 0x00;

    status |= FT_Write(this->m_device->GetHandle(), buffer, writeLength + 3, &sent);


    if (readLength > 0)
    {
        //this->m_device->Purge();

        this->sendStartCondition(address);

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
	else {
		this->sendStopCondition();
	}

    delete [] buffer;

    if((read != readLength) || (status != FT_OK))
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);

	return true;
}
