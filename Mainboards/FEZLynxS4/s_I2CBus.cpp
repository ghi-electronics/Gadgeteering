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
using namespace Mainboards;

FEZLynxS4::I2CBus::I2CBus(CPUPin sdaPin, CPUPin sclPin, FTDI_Device *device) : Interfaces::I2CBus(sdaPin, sclPin)
{
    this->m_device = device;
    this->m_bytesToSend = 0;
}

FEZLynxS4::I2CBus::~I2CBus()
{
}

void FEZLynxS4::I2CBus::sendStopCondition()
{
    char* buffer = new char[1024];
    DWORD sent = 0, read = 0, timeout = 0, dwNumBytesToSend = 0;
    FT_STATUS status;

	DWORD dwCount;

    buffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
    buffer[dwNumBytesToSend++] = '\x00'; //Set SDA low, SCL high, WP disabled by SK at bit „1‟, DO, GPIOL0 at bit „0‟
    buffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit „1‟, other pins as input with bit „0‟

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
    //for(dwCount=0; dwCount<4; dwCount++) // Repeat commands to ensure the minimum period of the stop hold time ie 600ns is achieved
    //{
        buffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
        buffer[dwNumBytesToSend++] = '\x00'; //Set SDA, SCL low, WP disabled by SK, DO at bit „1‟, GPIOL0 at bit „0‟
        buffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit „1‟, other pins as input with bit „0‟
    //}

	status = FT_Write(this->m_device->GetHandle(), buffer, dwNumBytesToSend, &sent); //Send off the commands
    dwNumBytesToSend = 0; //Clear output buffer

    delete [] buffer;

	return;
}

bool FEZLynxS4::I2CBus::sendStartCondition(unsigned char address)
{
    char* buffer = new char[1024];

    DWORD sent = 0, read = 0, timeout = 0, dwNumBytesToSend = 0, dwCount = 0;
    FT_STATUS status;

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
   // for(dwCount=0; dwCount<4; dwCount++) // Repeat commands to ensure the minimum period of the stop hold time ie 600ns is achieved
    //{
        buffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
        buffer[dwNumBytesToSend++] = '\x00'; //Set SDA, SCL low, WP disabled by SK, DO at bit „1‟, GPIOL0 at bit „0‟
        buffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit „1‟, other pins as input with bit „0‟
    //}

    status = FT_Write(this->m_device->GetHandle(), buffer, dwNumBytesToSend, &sent); //Send off the commands
    dwNumBytesToSend = 0; //Clear output buffer

    delete [] buffer;

    return true;
}

bool FEZLynxS4::I2CBus::transmit(bool sendStart, bool sendStop, unsigned char data) {
    unsigned char* writeBuffer = new unsigned char[1024];
    DWORD sent = 0, timeout = 0, dwNumBytesToSend = 0;
    FT_STATUS status;

    if(sendStart)
        this->sendStartCondition(data);

    writeBuffer[dwNumBytesToSend++] = 0x11;
    writeBuffer[dwNumBytesToSend++] = 0x00;
    writeBuffer[dwNumBytesToSend++] = 0x00;
    writeBuffer[dwNumBytesToSend++] = data;

    writeBuffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
    writeBuffer[dwNumBytesToSend++] = '\x00'; //Set SDA, SCL low, WP disabled by SK, DO, GPIOL0 at bit „0‟
    writeBuffer[dwNumBytesToSend++] = '\x10'; //Set SK,DO,GPIOL0 pins as output with bit „1‟, other pins as input with bit „0‟

    status |= FT_Write(this->m_device->GetHandle(), writeBuffer, dwNumBytesToSend, &sent);
    dwNumBytesToSend = 0;

    while(!(this->m_device->GetValue() & FTDI_PIN(this->scl)))
    {
        if(++timeout > 2000)
            mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR, 255);
    }

    bool nack = (this->m_device->GetValue() & FTDI_PIN(this->sda));

    //Reset pin-states to ensure next transmit is correct
    writeBuffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
    writeBuffer[dwNumBytesToSend++] = '\x00'; //Set SDA low, SCL high, WP disabled by SK at bit „1‟, DO, GPIOL0 at bit „0‟
    writeBuffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit „1‟, other pins as input with bit „0‟

    status = FT_Write(this->m_device->GetHandle(), writeBuffer, dwNumBytesToSend, &sent); //Send off the commands
    dwNumBytesToSend = 0; //Clear output buffer

    delete [] writeBuffer;

    if(sendStop)
        this->sendStopCondition();

    return nack;
}

unsigned char FEZLynxS4::I2CBus::receive(bool sendAcknowledgeBit, bool sendStop)
{
    DWORD read = 0, available = 0, sent = 0, dwNumBytesToSend = 0;
    unsigned char *buffer = new unsigned char[1024];

    FT_STATUS status = FT_OK;

    buffer[dwNumBytesToSend++] = 0x20;
    buffer[dwNumBytesToSend++] = 0;
    buffer[dwNumBytesToSend++] = 0;
    buffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
    buffer[dwNumBytesToSend++] = '\x00'; //Set SDA low, SCL high, WP disabled by SK at bit „1‟, DO, GPIOL0 at bit „0‟
    buffer[dwNumBytesToSend++] = '\x10'; //Set SK,DO,GPIOL0 pins as output with bit „1‟, other pins as input with bit „0‟

    status |= FT_Write(this->m_device->GetHandle(), buffer, dwNumBytesToSend, &sent);
    dwNumBytesToSend = 0;

    int timeout = 0;

    do {
        status |= FT_GetQueueStatus(this->m_device->GetHandle(), &available);
        System::Sleep(1);
        timeout++;
    } while (available < 1 && timeout < 2000);

    status |= FT_Read(this->m_device->GetHandle(), buffer, 1, &read);

    if(sendStop)
        this->sendStopCondition();

    if((read != 1) || (status != FT_OK))
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR, 254);

    unsigned char result = buffer[0];

    delete [] buffer;

    return result;
}

unsigned int FEZLynxS4::I2CBus::write(const unsigned char *buffer, unsigned int count, unsigned char address, bool sendStop)
{
    unsigned char* writeBuffer = new unsigned char[count + 3];
    DWORD sent = 0;
    FT_STATUS status = FT_OK;

    this->sendStartCondition(address);

    writeBuffer[0] = 0x10;
    writeBuffer[1] = (count - 1) & 0xFF;
    writeBuffer[2] = ((count - 1) >> 8) & 0xFF;
    for (int i = 0; i < count; i++)
        writeBuffer[i + 3] = buffer != NULL ? buffer[i] : 0x00;

    status |= FT_Write(this->m_device->GetHandle(), writeBuffer, count + 3, &sent);

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
        this->sendStopCondition();

    if((read != count) || (status != FT_OK))
        mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR, 254);

	return read;
}

bool FEZLynxS4::I2CBus::writeRead(const unsigned char *writeBuffer, unsigned int writeLength, unsigned char *readBuffer, unsigned int readLength, unsigned int *numWritten, unsigned int *numRead, unsigned char address)
{
    *numWritten = 0;
    *numRead = 0;

    unsigned int i = 0;
    unsigned int write = 0;
    unsigned int read = 0;

    if (writeLength > 0) {
        if (!this->transmit(true, false, address)) {
            for (i = 0; i < writeLength - 1; i++) {
                if (!this->transmit(false, false, writeBuffer[i])) {
                    (write)++;
                }
            }
        }

        if (!this->transmit(false, (readLength == 0), writeBuffer[i]))
            write++;

        *numWritten = write;
    }

    if (readLength > 0) {
        if (!this->transmit(true, false, address | 1)) {
            for (i = 0; i < readLength - 1; i++) {
                readBuffer[i] = this->receive(true, false);
                read++;
            }
        }

        readBuffer[i] = this->receive(false, true);
        read++;
        *numRead = read;
    }

    return (write + read) == (writeLength + readLength);
}

