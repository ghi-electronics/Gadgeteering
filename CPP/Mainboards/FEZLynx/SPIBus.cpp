#include <Gadgeteering\Gadgeteering.h>

#include "FEZLynx.h"

using namespace GHI::Interfaces;

byte SPIBus::writeReadByte(byte toSend, bool deselectChip)
{
	setClockHigh();
	dwNumBytesToSend = 0; //Clear output buffer
	OutputBuffer[dwNumBytesToSend++] = 0x10;//0x31 ; //Clock data byte out on +ve Clock Edge LSB first
	OutputBuffer[dwNumBytesToSend++] = 0;
	OutputBuffer[dwNumBytesToSend++] = 0; //Data length of 0x0000 means 1 byte data to clock out
	OutputBuffer[dwNumBytesToSend++] = (data); //Add data to be send
	ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); //Send off the commands

	dwNumBytesToSend = 0; //Clear output buffer
	OutputBuffer[dwNumBytesToSend++] = 0x81 ; //Clock data byte out on +ve Clock Edge LSB first
	InputBuffer[0] = 0;
	InputBuffer[1] = 0;

	FT_Read(ftHandle, InputBuffer, 2, &dwNumBytesRead); //Read out the data from FT2232H receive buffer

	if(dwNumBytesRead!=2)
	{
		//crap!
		while(1);
	}
}

void SPIBus::writeAndRead(byte* sendBuffer, byte* receiveBuffer, unsigned int count, bool deselectChip)
{

}

void SPIBus::writeThenRead(byte* sendBuffer, byte* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, bool deselectChip)
{

}

void SPIBus::write(byte* buffer, unsigned int count, bool deselectChip)
{

}

void SPIBus::read(byte* buffer, unsigned int count, bool deselectChip)
{

}