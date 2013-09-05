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

	this->MISO = new Interfaces::DigitalInput(socket->pins[8], GHI::ResistorModes::PULL_DOWN);
	mainboard->writeDigital(this->sck, true);
}

FEZLynx::SPIBus::SPIBus(CPUPin MISO, CPUPin MOSI, CPUPin SCK, FT_HANDLE Channel) : GHI::Interfaces::SPIBus(MOSI, MISO, SCK)
{
	channel = Channel;
}

FEZLynx::SPIBus::~SPIBus()
{

}

char FEZLynx::SPIBus::writeReadByte(char toSend, GHI::Interfaces::SPIConfiguration *configuration)
{
    char activeState = 0x10;

    if(!configuration->clockEdge)
        activeState = 0x33;
	else
		activeState = 0x36;

	mainboard->writeDigital(this->sck, configuration->clockEdge);

    dwNumBytesToSend = 0; //Clear output buffer
	OutputBuffer[dwNumBytesToSend++] = activeState;//0x31 ; //Clock data byte out on +ve Clock Edge LSB first
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
    /*for(unsigned int i = 0; i < count; i++)
        receiveBuffer[i] = writeReadByte(sendBuffer[i], configuration);*/

	write((const unsigned char*)sendBuffer,count,configuration);
}

void FEZLynx::SPIBus::writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, GHI::Interfaces::SPIConfiguration* configuration)
{
	unsigned char* rcv = new unsigned char[receiveCount];
    write((const unsigned char*)sendBuffer, sendCount, configuration);
    read(rcv,receiveCount, configuration);

	for(int i = 0; i < receiveCount; i++)
		receiveBuffer[i] = (char)rcv[i];
}

//#include "include\windows\libMPSSE_spi.h"
//FT_HANDLE handle = NULL;

void FEZLynx::SPIBus::writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, GHI::Interfaces::SPIConfiguration* configuration) 
{
	/*for(int i = 0; i < count; i++)
		this->writeByte(sendBuffer[i], configuration);*/

	write((const unsigned char*)sendBuffer,count,configuration);
	
	//FT_STATUS status;
	//uint8* buffer = (uint8*)sendBuffer;
	//uint8* bufferIn = receiveBuffer;
	//uint32 sent;
	//
	//if (handle == NULL) {
	//	Init_libMPSSE();
	//
	//	ChannelConfig config;
	//	config.ClockRate = 4000;
	//	config.LatencyTimer = 1;
	//	config.Pin = 0x00;
	//	config.configOptions = SPI_CONFIG_OPTION_CS_DBUS7 | SPI_CONFIG_OPTION_CS_ACTIVELOW | SPI_CONFIG_OPTION_MODE0;
	//
	//	status = SPI_OpenChannel(0, &handle);
	//	status = SPI_InitChannel(handle, &config);
	//}
	//
	//if (sendBuffer != NULL && receiveBuffer != NULL)
	//	status = SPI_ReadWrite(handle, bufferIn, buffer, count * 8, &sent, SPI_TRANSFER_OPTIONS_SIZE_IN_BITS | SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE | SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
	//else if (sendBuffer != NULL && receiveBuffer == NULL)
	//	status = SPI_Write(handle, buffer, count, &sent, SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE | SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
	//else if (sendBuffer == NULL && receiveBuffer != NULL)
	//	status = SPI_Read(handle, bufferIn, count, &sent, SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE | SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
}

void FEZLynx::SPIBus::write(const unsigned char* buffer, unsigned int count, GHI::Interfaces::SPIConfiguration* configuration)
{
	if(!count)
		return;

	if(buffer == NULL)
		return;

	//Get instance of FEZ Lynx
	FEZLynx *board = (FEZLynx *)mainboard;

	unsigned char mask = board->GetChannelMask(0);
	unsigned char direction = board->GetChannelDirection(0);

    char activeState = 0x10;

	//We must force the clock to be the oposite of the clock edge
	////before writing each byte
    if(!configuration->clockEdge)
	{
        activeState = 0x10;
		mask |= 0x01;
	}
	else
	{
		activeState = 0x11;
		mask &= (~0x01);
	}

    dwNumBytesToSend = 0; //Clear output buffer
	
    for(unsigned int i = 0; i < count; i++)
	{

		//OutputBuffer[dwNumBytesToSend++] = 0x80;
		//OutputBuffer[dwNumBytesToSend++] = mask;
		//OutputBuffer[dwNumBytesToSend++] = direction;

		OutputBuffer[dwNumBytesToSend++] = activeState;//0x31 ; //Clock data byte out on +ve Clock Edge LSB first
		OutputBuffer[dwNumBytesToSend++] = 0;
		OutputBuffer[dwNumBytesToSend++] = 0;
		OutputBuffer[dwNumBytesToSend++] = (char)buffer[i];
	}

    ftStatus = FT_Write(channel, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); //Send off the commands
}

void FEZLynx::SPIBus::writeByte(const unsigned char buffer, GHI::Interfaces::SPIConfiguration* configuration)
{
	/*if(buffer == NULL)
		return;*/

	//Get instance of FEZ Lynx
	FEZLynx *board = (FEZLynx *)mainboard;

	unsigned char mask = board->GetChannelMask(0);
	unsigned char direction = board->GetChannelDirection(0);

    char activeState = 0x10;

    if(!configuration->clockEdge)
        activeState = 0x10;
	else
		activeState = 0x11;

    dwNumBytesToSend = 0; //Clear output buffer

	mainboard->writeDigital(1, !configuration->clockEdge);

	OutputBuffer[dwNumBytesToSend++] = activeState;//0x31 ; //Clock data byte out on +ve Clock Edge LSB first
    OutputBuffer[dwNumBytesToSend++] = 0;
    OutputBuffer[dwNumBytesToSend++] = 0; //Data length of 0x0000 means 1 byte data to clock out
    OutputBuffer[dwNumBytesToSend++] = buffer;

    ftStatus = FT_Write(channel, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); //Send off the commands
}

void FEZLynx::SPIBus::read(unsigned char* buffer, unsigned int count, GHI::Interfaces::SPIConfiguration* configuration)
{
	if(buffer == NULL)
		return;

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
