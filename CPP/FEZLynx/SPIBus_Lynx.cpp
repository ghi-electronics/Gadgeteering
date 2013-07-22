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

using namespace GHI;
using namespace GHI::Mainboards;

FEZLynx::SPIBus::SPIBus(Socket* socket, FT_HANDLE Channel) : GHI::Interfaces::SPIBus(socket->pins[7], socket->pins[8], socket->pins[9])
{
	channel = Channel;
}

FEZLynx::SPIBus::~SPIBus()
{
}

char FEZLynx::SPIBus::writeReadByte(char toSend, GHI::Interfaces::SPIDevice::Configuration* configuration) {
	dwNumBytesToSend = 0; //Clear output buffer
	OutputBuffer[dwNumBytesToSend++] = 0x10;//0x31 ; //Clock data byte out on +ve Clock Edge LSB first
	OutputBuffer[dwNumBytesToSend++] = 0;
	OutputBuffer[dwNumBytesToSend++] = 0; //Data length of 0x0000 means 1 byte data to clock out
	OutputBuffer[dwNumBytesToSend++] = (toSend); //Add data to be send
	ftStatus = FT_Write(channel, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); //Send off the commands

	dwNumBytesToSend = 0; //Clear output buffer
	OutputBuffer[dwNumBytesToSend++] = 0x81 ; //Clock data byte out on +ve Clock Edge LSB first
	InputBuffer[0] = 0;

	FT_Read(channel, InputBuffer, 1, &dwNumBytesRead); //Read out the data from FT2232H receive buffer

	if(dwNumBytesRead!=1)
	{
		mainboard->panic(0x20);
	}

	return InputBuffer[0];
}

void FEZLynx::SPIBus::writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration) {

}

void FEZLynx::SPIBus::writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, GHI::Interfaces::SPIDevice::Configuration* configuration) {

}

void FEZLynx::SPIBus::write(char* buffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration) {

}

void FEZLynx::SPIBus::read(char* buffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration) {

}