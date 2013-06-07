#include "FEZLynx.h"
#include "../Gadgeteering/System.hpp"

#include <iostream>

using namespace GHI::Mainboards;
using namespace std;

FEZLynx::ExtendedSockets::ExtendedSockets(FT_HANDLE Channel, unsigned char Address, Socket* socket)
{
	SCL = new GHI::Interfaces::DigitalInputOutput(socket, 9, GHI::IOStates::DIGITAL_OUTPUT, false);
	SDA = new GHI::Interfaces::DigitalInputOutput(socket, 8, GHI::IOStates::DIGITAL_OUTPUT, false);

	address = Address;
	channel = Channel;
}

void FEZLynx::ExtendedSockets::pullSCLHigh()
{
	SCL->setState(GHI::IOStates::DIGITAL_INPUT);
}
void FEZLynx::ExtendedSockets::pullSCLLow()
{
	SCL->setState(GHI::IOStates::DIGITAL_OUTPUT);
}

void FEZLynx::ExtendedSockets::pullSDALow() 
{
	SDA->setState(GHI::IOStates::DIGITAL_OUTPUT);
}

void FEZLynx::ExtendedSockets::pullSDAHigh()
{
	SDA->setState(GHI::IOStates::DIGITAL_INPUT);
}

bool FEZLynx::ExtendedSockets::readSCL()
{
	dwNumBytesToSend=0;
	OutputBuffer[dwNumBytesToSend++] = '\x81'; //Set Command to read pins
	ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); //Send off the commands

	dwNumBytesToSend = 0; //Clear output buffer

	ftStatus = FT_Read(ftHandle, InputBuffer, 1, &dwNumBytesRead); //Read one byte from device receive buffer

	if ((ftStatus != FT_OK) || (dwNumBytesRead == 0))
	{
		cout << "Status: " << ftStatus << endl;
		return false; /*Error, can't get the ACK bit from EEPROM */ 
	}
	else
	{
		if (((InputBuffer[0] & 0x01) == 0x00) )
			return false;
		else
			return true;
	}
}

bool FEZLynx::ExtendedSockets::readSDA()
{
	dwNumBytesToSend=0;
	OutputBuffer[dwNumBytesToSend++] = '\x81'; //Set Command to read pins
	ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); //Send off the commands

	dwNumBytesToSend = 0; //Clear output buffer
	ftStatus = FT_Read(ftHandle, InputBuffer, 1, &dwNumBytesRead); //Read one byte from device receive buffer

	if ((ftStatus != FT_OK) || (dwNumBytesRead == 0))
	{
		cout << "Status: " << ftStatus << endl;
		return false; /*Error, can't get the ACK bit from EEPROM */ 
	}
	else
	{
		if (((InputBuffer[0] & 0x02) == 0x00) )
			return false;
		else
			return true;
	}
}

void FEZLynx::ExtendedSockets::waitForSCL() {
	pullSCLHigh();

	unsigned long endTime = GHI::System::TimeElapsed64() + 2000;
	while(!readSCL());// && micros() < endTime)	;
}

bool FEZLynx::ExtendedSockets::sendStartCondition(unsigned char startAddress) {
	pullSDALow();
	pullSCLLow();
	
//	pullSDAHigh();
	//pullSCLHigh();
	return Write(startAddress);
}

void FEZLynx::ExtendedSockets::sendStopCondition() {
	waitForSCL();
	pullSDAHigh();
	//pullSCLHigh();
}

bool FEZLynx::ExtendedSockets::Write(byte data) {
	//FT_STATUS ftStatus = FT_OK;
	dwNumBytesToSend=0;
	OutputBuffer[dwNumBytesToSend++] = MSB_FALLING_EDGE_CLOCK_BYTE_OUT; //Clock data byte out on –ve Clock Edge MSB first
	OutputBuffer[dwNumBytesToSend++] = '\x00';
	OutputBuffer[dwNumBytesToSend++] = '\x00'; //Data length of 0x0000 means 1 byte data to clock out
	OutputBuffer[dwNumBytesToSend++] = data; //Add data to be send
	ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); //Send off the commands

	pullSDAHigh();
	waitForSCL();

	bool sda = readSDA();

	pullSCLLow();
	pullSDALow();

	if (sda != 0)
		sendStopCondition();

	return sda == 0;
}

bool FEZLynx::ExtendedSockets::writeRegisters(unsigned char startAddress, unsigned char count, unsigned char* data) {
	if (!sendStartCondition(0x40)) 
	{
		sendStopCondition();
		return false;
	}
	Write(startAddress);
	for (int i = 0; i < count; i++)
		if (!Write(data[i]))  
			return false;

	sendStopCondition();

	return true;
}

bool FEZLynx::ExtendedSockets::writeRegister(unsigned char location, unsigned char data) {
	return writeRegisters(location, 1, &data);
}

unsigned char FEZLynx::ExtendedSockets::read(bool isLast) {
	pullSDAHigh();
	
	byte result = 0;
	for (byte i = 0; i < 8; i++) {
		result <<= 1;

		waitForSCL();

		if (readSDA()) 
			result |= 1;

		pullSCLLow();
	}

	if (!isLast)  
		pullSDALow();
		
	waitForSCL();
	pullSCLLow();

	if (isLast) 
		pullSDAHigh();

	return result;
}

bool FEZLynx::ExtendedSockets::readRegisters(unsigned char startAddress, unsigned char count, unsigned char* data) {
	if (!writeRegisters(startAddress, 0, NULL))
		return false;

	pullSCLLow();
	waitForSCL();

	sendStopCondition();

	if (!sendStartCondition(0x41))
		return false;

	for (byte i = 0; i < count - 1; i++)
		data[i] = read(false);

	data[count - 1] = read(true);

	sendStopCondition();

	return true;
}

unsigned char FEZLynx::ExtendedSockets::readRegister(unsigned char location) {
	byte data = 0;
	readRegisters(location, 1, &data);
	return data;
}