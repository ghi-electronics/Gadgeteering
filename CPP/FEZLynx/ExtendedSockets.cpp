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

#include <iostream>

using namespace GHI;
using namespace GHI::Mainboards;
using namespace std;

FEZLynx::ExtendedSockets::ExtendedSockets(FT_HANDLE Channel, unsigned char Address, Socket *socket)
{
	SCL = new GHI::Interfaces::DigitalIO(socket, 9);
	SDA = new GHI::Interfaces::DigitalIO(socket, 8);

	//clearSCL();
	//clearSDA();

	SCL->write(false);
	SDA->write(false);

	address = Address;
	ftHandle = Channel;
}

//void FEZLynx::ExtendedSockets::clearSCL()
//{
//	SCL->setIOState(GHI::IOStates::DIGITAL_OUTPUT);
//}
//
//void FEZLynx::ExtendedSockets::clearSDA()
//{
//	SDA->setIOState(GHI::IOStates::DIGITAL_OUTPUT);
//}
//
//bool FEZLynx::ExtendedSockets::readSCL()
//{
//	return SCL->read();
//}
//
//bool FEZLynx::ExtendedSockets::readSDA()
//{
//	return SDA->read();
//}
//
//bool FEZLynx::ExtendedSockets::sendStartCondition() {
//	if (this->start) {
//		this->readSDA();
//
//		unsigned long endTime = System::TimeElapsed() + 5000;
//		while (!this->readSCL() && System::TimeElapsed() < endTime)
//			;
//
//	}
//
//    if (!this->readSDA())
//		return false;
//
//	this->clearSDA();
//	this->clearSCL();
//
//	this->start = true;
//
//	return true;
//}
//
//bool FEZLynx::ExtendedSockets::writeBit(bool bit) {
//    if (bit)
//		this->readSDA();
//    else
//		this->clearSDA();
//
//	unsigned long endTime = System::TimeElapsed() + 5000;
//	while (!this->readSCL() && System::TimeElapsed() < endTime)
//		;
//	
//    if (bit && !this->readSDA())
//		return false;
//	
//	this->clearSCL();
//
//    return true;
//}
//
//bool FEZLynx::ExtendedSockets::readBit() {
//    this->readSDA();
//	
//	unsigned long endTime = System::TimeElapsed() + 5000;
//	while (!this->readSCL() && System::TimeElapsed() < endTime)
//		;
//
//    bool bit = this->readSDA();
//	
//	this->clearSCL();
//
//    return bit;
//}
//
//bool FEZLynx::ExtendedSockets::sendStopCondition() {
//	this->clearSDA();
//
//	unsigned long endTime = System::TimeElapsed() + 5000;
//	while (!this->readSCL() && System::TimeElapsed() < endTime)
//		;
//
//	if (!this->readSDA())
//		return false;
//	
//	this->start = false;
//
//	return true;
//}
//
//bool FEZLynx::ExtendedSockets::transmit(bool sendStart, bool sendStop, unsigned char data) {
//	unsigned int bit;
//	bool nack;
//	
//	if (sendStart)
//		this->sendStartCondition();
//	
//    /*for (bit = 0; bit < 8; bit++) {
//		this->writeBit((data & 0x80) != 0);
//
//		data <<= 1;
//    }*/
//    
//    nack = this->readBit();
//
//	if (sendStop)
//		this->sendStopCondition();
//	
//     return nack;
//}
//
//unsigned char FEZLynx::ExtendedSockets::receive(bool sendAcknowledgeBit, bool sendStopCondition) {
//	unsigned char d = 0;
//	unsigned int bit = 0;
//
//	for (bit = 0; bit < 8; bit++) {
//		d <<= 1;
//
//		if (this->readBit())
//			d |= 1;
//	}
//
//	this->writeBit(sendAcknowledgeBit);
//
//	if (sendStopCondition)
//		this->sendStopCondition();
//
//	return d;
//}
//
//unsigned int FEZLynx::ExtendedSockets::writeBytes(unsigned char* data, unsigned int length) {
//	if (!length) 
//		return 0;
//
//	DWORD numWrite = 0;
//	unsigned int i = 0;
//	
//	/*if (!this->transmit(true, false, this->address))
//		for (i = 0; i < length - 1; i++)
//			if (!this->transmit(false, false, data[i]))
//				numWrite++;
//	
//	if (!this->transmit(false, true, data[i]))
//		numWrite++;*/
//
//	dwNumBytesToSend = 0; //Clear output buffer
//	OutputBuffer[dwNumBytesToSend++] = 0x11; //Clock data byte out on +ve Clock Edge LSB first
//	OutputBuffer[dwNumBytesToSend++] = 0;
//	OutputBuffer[dwNumBytesToSend++] = length; //Data length of 0x0000 means 1 byte data to clock out
//	OutputBuffer[dwNumBytesToSend++] = this->address;
//
//	for(int i = 0; i < length; i++)
//		OutputBuffer[dwNumBytesToSend++] = data[i]; //Add data to be sent
//
//	ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &numWrite); //Send off the commands
//	
//	return numWrite;
// }
//
//unsigned int FEZLynx::ExtendedSockets::readBytes(unsigned char* data, unsigned int length) {	
//	
//	if (!length) 
//		return 0;
//
//	unsigned int numRead = 0;
//	unsigned int i = 0;
//
//	DWORD bytesAvail = 0;
//	DWORD numWrite = 0;
//
//	dwNumBytesToSend = 0; //Clear output buffer
//	OutputBuffer[dwNumBytesToSend++] = 0x20;//0x31 ; //Clock data byte out on +ve Clock Edge LSB first
//	OutputBuffer[dwNumBytesToSend++] = 0;
//	OutputBuffer[dwNumBytesToSend++] = 0;
//
//	ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &numWrite); //Send off the commands
//
//	FT_STATUS status = FT_GetQueueStatus(ftHandle, &bytesAvail);
//
//	if((status == FT_OK) && bytesAvail > 0)
//	{
//		DWORD bytesRead = 0;
//		
//		status = FT_Read(ftHandle,data,length, &bytesRead);
//
//		if(status != FT_OK)
//			return 0;
//
//		numRead = (unsigned int)bytesRead;
//	}
//
//    return numRead;
//}
//
//bool FEZLynx::ExtendedSockets::writeRead(unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength, unsigned int* numWritten, unsigned int* numRead) {
//	*numWritten = 0;
//	*numRead = 0;
//
//	unsigned int i = 0;
//	unsigned char write = 0;
//	unsigned char read = 0;
//
//    if (writeLength > 0) {
//
//		DWORD numWrite = 0;
//
//		dwNumBytesToSend = 0; //Clear output buffer
//		OutputBuffer[dwNumBytesToSend++] = 0x31 ; //Clock data byte out on +ve Clock Edge LSB first
//		OutputBuffer[dwNumBytesToSend++] = 0;
//		OutputBuffer[dwNumBytesToSend++] = writeLength; //Data length of 0x0000 means 1 byte data to clock out
//		OutputBuffer[dwNumBytesToSend++] = this->address;
//
//		for(int i = 0; i < writeLength; i++)
//			OutputBuffer[dwNumBytesToSend++] = writeBuffer[i]; //Add data to be sent
//
//		ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &numWrite); //Send off the commands
//
//		*numWritten = numWrite;
//
//		write = (numWrite - 1); //Offset for address byte
//    }
//
//    if (readLength > 0) {
//		DWORD bytesAvail = 0;
//
//		FT_STATUS status = FT_GetQueueStatus(ftHandle, &bytesAvail);
//
//		if((status == FT_OK) && bytesAvail > 0)
//		{
//			DWORD bytesRead = 0;
//		
//			status = FT_Read(ftHandle,readBuffer,readLength, &bytesRead);
//
//			if(status != FT_OK)
//				return 0;
//
//			*numRead = (unsigned int)bytesRead;
//
//			read = bytesRead;
//		}
//    }
//
//	return (write + read) == (writeLength + readLength);
//}
//
//bool FEZLynx::ExtendedSockets::writeRegister(unsigned char registerAddress, unsigned char value) {
//	unsigned char data[2] = {registerAddress, value};
//	return this->writeBytes(data, 2) == 2;
//}
//
//unsigned char FEZLynx::ExtendedSockets::readRegister(unsigned char registerAddress) {
//	unsigned char value = 0xFF;
//	unsigned int written, read;
//
//	this->writeRead(&registerAddress, 1, &value, 1, &written, &read);
//
//	return value;
//}
//
//unsigned char FEZLynx::ExtendedSockets::getPort(GHI::CPUPin pinNumber) {
//
//	if((pinNumber & FEZLynx::Port1Mask) == FEZLynx::Port1Mask)
//		return 0;
//
//	if((pinNumber & FEZLynx::Port2Mask) == FEZLynx::Port2Mask)
//		return 1;
//
//	if((pinNumber & FEZLynx::Port3Mask) == FEZLynx::Port3Mask)
//		return 2;
//
//	if((pinNumber & FEZLynx::Port4Mask) == FEZLynx::Port4Mask)
//		return 3;
//
//	if((pinNumber & FEZLynx::Port5Mask) == FEZLynx::Port5Mask)
//		return 4;
//
//	if((pinNumber & FEZLynx::Port6Mask) == FEZLynx::Port6Mask)
//		return 5;
//
//	if((pinNumber & FEZLynx::Port7Mask) == FEZLynx::Port7Mask)
//		return 6;
//
//	if((pinNumber & FEZLynx::Port8Mask) == FEZLynx::Port8Mask)
//		return 7;
//
//	mainboard->panic(Exceptions::ERR_PORT_OUT_OF_RANGE);
//
//	return NULL;
//}
//
//unsigned char FEZLynx::ExtendedSockets::getPin(GHI::CPUPin pinNumber) {
//	pinNumber &= ~FEZLynx::ExtenderMask;
//
//	if((pinNumber & FEZLynx::Port1Mask) == FEZLynx::Port1Mask)
//		return pinNumber &= ~FEZLynx::Port1Mask;
//
//	if((pinNumber & FEZLynx::Port2Mask) == FEZLynx::Port2Mask)
//		return pinNumber &= ~FEZLynx::Port2Mask;
//
//	if((pinNumber & FEZLynx::Port3Mask) == FEZLynx::Port3Mask)
//		return pinNumber &= ~FEZLynx::Port3Mask;
//
//	if((pinNumber & FEZLynx::Port4Mask) == FEZLynx::Port4Mask)
//		return pinNumber &= ~FEZLynx::Port4Mask;
//
//	if((pinNumber & FEZLynx::Port5Mask) == FEZLynx::Port5Mask)
//		return pinNumber &= ~FEZLynx::Port5Mask;
//
//	if((pinNumber & FEZLynx::Port6Mask) == FEZLynx::Port6Mask)
//		return pinNumber &= ~FEZLynx::Port6Mask;
//
//	if((pinNumber & FEZLynx::Port7Mask) == FEZLynx::Port7Mask)
//		return pinNumber &= ~FEZLynx::Port7Mask;
//
//	if((pinNumber & FEZLynx::Port8Mask) == FEZLynx::Port8Mask)
//		return pinNumber &= ~FEZLynx::Port8Mask;
//
//	mainboard->panic(Exceptions::ERR_PIN_OUT_OF_RANGE);
//
//	return NULL;
//}
//
//void FEZLynx::ExtendedSockets::setIOMode(GHI::CPUPin pinNumber, IOState state, ResistorMode resistorMode) {
//	this->writeRegister(FEZLynx::ExtendedSockets::PORT_SELECT_REGISTER, this->getPort(pinNumber));
//
//	char pin = this->getPin(pinNumber);
//	char val = this->readRegister(FEZLynx::ExtendedSockets::ENABLE_PWM_REGISTER);
//
//	if (state == IOStates::PWM)	{
//		this->writeRegister(FEZLynx::ExtendedSockets::ENABLE_PWM_REGISTER, val | pin);
//
//		this->writeRegister(FEZLynx::ExtendedSockets::PWM_CONFIG, FEZLynx::ExtendedSockets::CLOCK_SOURCE); //93.75KHz clock
//
//		this->writeDigital(pin, true);
//	}
//	else {
//		this->writeRegister(FEZLynx::ExtendedSockets::ENABLE_PWM_REGISTER, val & ~pin);
//		val = this->readRegister(FEZLynx::ExtendedSockets::PIN_DIRECTION_REGISTER);
//
//		if (state == IOStates::DIGITAL_INPUT) {
//			char resistorRegister = FEZLynx::ExtendedSockets::PIN_HIGH_IMPEDENCE;
//			if (resistorMode == ResistorModes::PULL_DOWN)
//				resistorRegister = FEZLynx::ExtendedSockets::PIN_PULL_DOWN;
//			else if (resistorMode == ResistorModes::PULL_UP)
//				resistorRegister = FEZLynx::ExtendedSockets::PIN_PULL_UP;
//
//			this->writeRegister(FEZLynx::ExtendedSockets::PIN_DIRECTION_REGISTER, val | pin);
//			val = this->readRegister(resistorRegister);
//			this->writeRegister(resistorRegister, val | pin);
//		}
//		else {
//			this->writeRegister(FEZLynx::ExtendedSockets::PIN_DIRECTION_REGISTER, val & ~pin);
//			val = this->readRegister(FEZLynx::ExtendedSockets::PIN_STRONG_DRIVE);
//			this->writeRegister(FEZLynx::ExtendedSockets::PIN_STRONG_DRIVE, val | pin);
//		}
//	}
//}
//
////We're using the 93.75KHz clock source because it gives a good resolution around the 1KHz frequency
////while still allowing the user to select frequencies such as 10KHz, but with reduced duty cycle
////resolution.
//void FEZLynx::ExtendedSockets::setPWM(GHI::CPUPin pin, double frequency, double dutyCycle) {
//	this->writeRegister(FEZLynx::ExtendedSockets::PWM_SELECT_REGISTER, (char)((pin % 8) + (this->getPort(pin) - 6) * 8));
//	
//	char period = (char)(93750 / frequency);
//
//	this->writeRegister(FEZLynx::ExtendedSockets::PERIOD_REGISTER, period);
//	this->writeRegister(FEZLynx::ExtendedSockets::PULSE_WIDTH_REGISTER, period * dutyCycle);
//}
//
//bool FEZLynx::ExtendedSockets::readDigital(GHI::CPUPin pin) {
//	char b = this->readRegister(FEZLynx::ExtendedSockets::INPUT_PORT_0_REGISTER + this->getPort(pin));
//
//	return b & this->getPin(pin);
//}
//
//void FEZLynx::ExtendedSockets::writeDigital(GHI::CPUPin pin, bool value) {
//	unsigned int port = this->getPort(pin);
//
//	unsigned char b = this->readRegister(FEZLynx::ExtendedSockets::OUTPUT_PORT_0_REGISTER + port);
//
//	if (value)
//		b |= this->getPin(pin);
//	else
//		b &= ~this->getPin(pin);
//
//	this->writeRegister(FEZLynx::ExtendedSockets::OUTPUT_PORT_0_REGISTER + port, b);
//}
//
//double FEZLynx::ExtendedSockets::readAnalog(GHI::CPUPin pin) {
//	mainboard->panic(Exceptions::ERR_PWM_NOT_SUPPORTED);
//
//	return 0.0;
//}
//
//void FEZLynx::ExtendedSockets::writeAnalog(GHI::CPUPin pin, double voltage) {
//	mainboard->panic(Exceptions::ERR_PWM_NOT_SUPPORTED);
//
//}

void FEZLynx::ExtendedSockets::pullSCLHigh()
{
	SCL->setIOState(GHI::IOStates::DIGITAL_INPUT);
}
void FEZLynx::ExtendedSockets::pullSCLLow()
{
	SCL->setIOState(GHI::IOStates::DIGITAL_OUTPUT);
}

void FEZLynx::ExtendedSockets::pullSDALow() 
{
	SDA->setIOState(GHI::IOStates::DIGITAL_OUTPUT);
}

void FEZLynx::ExtendedSockets::pullSDAHigh()
{
	SDA->setIOState(GHI::IOStates::DIGITAL_INPUT);
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

	unsigned long endTime = GHI::System::TimeElapsed() + 2000;
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

bool FEZLynx::ExtendedSockets::Write(unsigned char data) {
	//FT_STATUS ftStatus = FT_OK;
	dwNumBytesToSend=0;
	OutputBuffer[dwNumBytesToSend++] = 0x31;//MSB_FALLING_EDGE_CLOCK_BYTE_OUT; //Clock data byte out on –ve Clock Edge MSB first
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

unsigned char FEZLynx::ExtendedSockets::getPort(CPUPin pinNumber) {

	if((pinNumber & FEZLynx::Port1Mask) == FEZLynx::Port1Mask)
		return 0;

	if((pinNumber & FEZLynx::Port2Mask) == FEZLynx::Port2Mask)
		return 1;

	if((pinNumber & FEZLynx::Port3Mask) == FEZLynx::Port3Mask)
		return 2;

	if((pinNumber & FEZLynx::Port4Mask) == FEZLynx::Port4Mask)
		return 3;

	if((pinNumber & FEZLynx::Port5Mask) == FEZLynx::Port5Mask)
		return 4;

	if((pinNumber & FEZLynx::Port6Mask) == FEZLynx::Port6Mask)
		return 5;

	if((pinNumber & FEZLynx::Port7Mask) == FEZLynx::Port7Mask)
		return 6;

	if((pinNumber & FEZLynx::Port8Mask) == FEZLynx::Port8Mask)
		return 7;

	mainboard->panic(Exceptions::ERR_PORT_OUT_OF_RANGE);

	return NULL;
}

unsigned char FEZLynx::ExtendedSockets::getPin(CPUPin pinNumber) {
	pinNumber &= ~FEZLynx::ExtenderMask;

	if((pinNumber & FEZLynx::Port1Mask) == FEZLynx::Port1Mask)
		return pinNumber &= ~FEZLynx::Port1Mask;

	if((pinNumber & FEZLynx::Port2Mask) == FEZLynx::Port2Mask)
		return pinNumber &= ~FEZLynx::Port2Mask;

	if((pinNumber & FEZLynx::Port3Mask) == FEZLynx::Port3Mask)
		return pinNumber &= ~FEZLynx::Port3Mask;

	if((pinNumber & FEZLynx::Port4Mask) == FEZLynx::Port4Mask)
		return pinNumber &= ~FEZLynx::Port4Mask;

	if((pinNumber & FEZLynx::Port5Mask) == FEZLynx::Port5Mask)
		return pinNumber &= ~FEZLynx::Port5Mask;

	if((pinNumber & FEZLynx::Port6Mask) == FEZLynx::Port6Mask)
		return pinNumber &= ~FEZLynx::Port6Mask;

	if((pinNumber & FEZLynx::Port7Mask) == FEZLynx::Port7Mask)
		return pinNumber &= ~FEZLynx::Port7Mask;

	if((pinNumber & FEZLynx::Port8Mask) == FEZLynx::Port8Mask)
		return pinNumber &= ~FEZLynx::Port8Mask;

	mainboard->panic(Exceptions::ERR_PORT_OUT_OF_RANGE);

	return NULL;
}

void FEZLynx::ExtendedSockets::setIOMode(CPUPin pinNumber, IOState state, ResistorMode resistorMode) {
	this->writeRegister(FEZLynx::ExtendedSockets::PORT_SELECT_REGISTER, this->getPort(pinNumber));

	char pin = this->getPin(pinNumber);
	char val = this->readRegister(FEZLynx::ExtendedSockets::ENABLE_PWM_REGISTER);

	if (state == IOStates::PWM)	{
		this->writeRegister(FEZLynx::ExtendedSockets::ENABLE_PWM_REGISTER, val | pin);

		this->writeRegister(FEZLynx::ExtendedSockets::PWM_CONFIG, FEZLynx::ExtendedSockets::CLOCK_SOURCE); //93.75KHz clock

		this->writeDigital(pin, true);
	}
	else {
		this->writeRegister(FEZLynx::ExtendedSockets::ENABLE_PWM_REGISTER, val & ~pin);
		val = this->readRegister(FEZLynx::ExtendedSockets::PIN_DIRECTION_REGISTER);

		if (state == IOStates::DIGITAL_INPUT) {
			char resistorRegister = FEZLynx::ExtendedSockets::PIN_HIGH_IMPEDENCE;
			if (resistorMode == ResistorModes::PULL_DOWN)
				resistorRegister = FEZLynx::ExtendedSockets::PIN_PULL_DOWN;
			else if (resistorMode == ResistorModes::PULL_UP)
				resistorRegister = FEZLynx::ExtendedSockets::PIN_PULL_UP;

			this->writeRegister(FEZLynx::ExtendedSockets::PIN_DIRECTION_REGISTER, val | pin);
			val = this->readRegister(resistorRegister);
			this->writeRegister(resistorRegister, val | pin);
		}
		else {
			this->writeRegister(FEZLynx::ExtendedSockets::PIN_DIRECTION_REGISTER, val & ~pin);
			val = this->readRegister(FEZLynx::ExtendedSockets::PIN_STRONG_DRIVE);
			this->writeRegister(FEZLynx::ExtendedSockets::PIN_STRONG_DRIVE, val | pin);
		}
	}
}

//We're using the 93.75KHz clock source because it gives a good resolution around the 1KHz frequency
//while still allowing the user to select frequencies such as 10KHz, but with reduced duty cycle
//resolution.
void FEZLynx::ExtendedSockets::setPWM(CPUPin pin, double frequency, double dutyCycle) {
	this->writeRegister(FEZLynx::ExtendedSockets::PWM_SELECT_REGISTER, (char)((pin % 8) + (this->getPort(pin) - 6) * 8));
	
	char period = (char)(93750 / frequency);

	this->writeRegister(FEZLynx::ExtendedSockets::PERIOD_REGISTER, period);
	this->writeRegister(FEZLynx::ExtendedSockets::PULSE_WIDTH_REGISTER, static_cast<unsigned char>(period * dutyCycle));
}

bool FEZLynx::ExtendedSockets::readDigital(CPUPin pin) {
	char b = this->readRegister(FEZLynx::ExtendedSockets::INPUT_PORT_0_REGISTER + this->getPort(pin));

	return (b & this->getPin(pin)) != 0;
}

void FEZLynx::ExtendedSockets::writeDigital(CPUPin pin, bool value) {
	char b = this->readRegister(FEZLynx::ExtendedSockets::OUTPUT_PORT_0_REGISTER + this->getPort(pin));

	if (value)
		b |= this->getPin(pin);
	else
		b &= ~this->getPin(pin);

	this->writeRegister(FEZLynx::ExtendedSockets::OUTPUT_PORT_0_REGISTER + this->getPort(pin), b);
}

double FEZLynx::ExtendedSockets::readAnalog(CPUPin pin) {
	mainboard->panic(Exceptions::ERR_READ_ANALOG_NOT_SUPPORTED);

	return 0.0;
}

void FEZLynx::ExtendedSockets::writeAnalog(CPUPin pin, double voltage) {
	mainboard->panic(Exceptions::ERR_WRITE_ANALOG_NOT_SUPPORTED);
}