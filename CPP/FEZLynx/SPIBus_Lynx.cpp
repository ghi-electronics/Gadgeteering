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

FEZLynx::SPIBus::SPIBus(Socket* socket, FT_HANDLE Channel) : GHI::Interfaces::SPIBus(socket->pins[7], socket->pins[8], socket->pins[9])
{
	channel = Channel;
}

FEZLynx::SPIBus::~SPIBus()
{
}

char FEZLynx::SPIBus::writeReadByte(char toSend, GHI::Interfaces::SPIConfiguration *configuration)
{
    char activeState = 0x10;

    if(configuration->chipSelectActiveState && configuration->clockEdge)
        activeState = 0x10;

    dwNumBytesToSend = 0; //Clear output buffer
    OutputBuffer[dwNumBytesToSend++] = 0x10;//0x31 ; //Clock data byte out on +ve Clock Edge LSB first
    OutputBuffer[dwNumBytesToSend++] = 0;
    OutputBuffer[dwNumBytesToSend++] = 0; //Data length of 0x0000 means 1 byte data to clock out
    OutputBuffer[dwNumBytesToSend++] = toSend;

    ftStatus = FT_Write(channel, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); //Send off the commands

    DWORD dwBytesInQueue = 0;
    int timeout = 0;
    ftStatus = FT_OK;

    //wait for queue to fill to desired amount, or timeout
    while((dwBytesInQueue < 1) && (timeout < 500))
    {
        ftStatus |= FT_GetQueueStatus(channel, &dwBytesInQueue);
        System::Sleep(1);
        timeout++;
    }

    if((timeout >= 499) || (ftStatus != FT_OK))
        mainboard->panic(0x25);

    dwNumBytesRead = 0;
    ftStatus = FT_Read(channel, InputBuffer, 1, &dwNumBytesRead);

    if((dwNumBytesRead != 1) || (ftStatus != FT_OK))
        mainboard->panic(0x25);

	return InputBuffer[0];
}

void FEZLynx::SPIBus::writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count, GHI::Interfaces::SPIConfiguration* configuration)
{
    for(unsigned int i = 0; i < count; i++)
        receiveBuffer[i] = writeReadByte(sendBuffer[i], configuration);
}

void FEZLynx::SPIBus::writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, GHI::Interfaces::SPIConfiguration* configuration)
{
    write(sendBuffer, sendCount, configuration);
    read(receiveBuffer,receiveCount, configuration);
}

void FEZLynx::SPIBus::write(char* buffer, unsigned int count, GHI::Interfaces::SPIConfiguration* configuration)
{
    char activeState = 0x10;

    if(configuration->chipSelectActiveState && configuration->clockEdge)
        activeState = 0x10;

    dwNumBytesToSend = 0; //Clear output buffer
    OutputBuffer[dwNumBytesToSend++] = 0x10;//0x31 ; //Clock data byte out on +ve Clock Edge LSB first
    OutputBuffer[dwNumBytesToSend++] = 0;
    OutputBuffer[dwNumBytesToSend++] = count - 1; //Data length of 0x0000 means 1 byte data to clock out

    for(int i = 0; i < count; i++)
        OutputBuffer[dwNumBytesToSend++] = (char)buffer[i];

    ftStatus = FT_Write(channel, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); //Send off the commands
}

void FEZLynx::SPIBus::read(char* buffer, unsigned int count, GHI::Interfaces::SPIConfiguration* configuration)
{
    DWORD dwBytesInQueue = 0;
    int timeout = 0;
    ftStatus = FT_OK;

    //wait for queue to fill to desired amount, or timeout
    while((dwBytesInQueue < count) && (timeout < 500))
    {
        ftStatus |= FT_GetQueueStatus(channel, &dwBytesInQueue);
        System::Sleep(1);
        timeout++;
    }

    if((timeout >= 499) || (ftStatus != FT_OK))
        mainboard->panic(0x25);

    dwNumBytesRead = 0;
    ftStatus = FT_Read(channel, buffer, count, &dwNumBytesRead);

    if((dwNumBytesRead != count) || (ftStatus != FT_OK))
        mainboard->panic(0x25);
}
