#include <Gadgeteering\Gadgeteering.h>

#include "FEZLynx.h"

using namespace GHI::Mainboards;

FEZLynx::SPIBus::SPIBus(FEZLynx::SPIBus::Configuration config) : GHI::Interfaces::SPIBus(config.MISO, config.MOSI, config.CLOCK)
{
	channel = config.Channel;
}

char FEZLynx::SPIBus::writeReadByte(char toSend, bool deselectChip)
{
	CLOCK.write(true);

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
		//crap!
		while(1);
	}

	CLOCK.write(false);

	return InputBuffer[0];
}

void FEZLynx::SPIBus::writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count, bool deselectChip)
{

}

void FEZLynx::SPIBus::writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, bool deselectChip)
{

}

void FEZLynx::SPIBus::write(char* buffer, unsigned int count, bool deselectChip)
{

}

void FEZLynx::SPIBus::read(char* buffer, unsigned int count, bool deselectChip)
{

}