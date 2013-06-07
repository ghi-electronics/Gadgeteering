#include "FEZLynx.h"
#include "../Gadgeteering/System.hpp"

#include <iostream>

using namespace GHI;
using namespace GHI::Mainboards;
using namespace std;

FEZLynx::ExtendedSockets::ExtendedSockets(FT_HANDLE Channel, unsigned char Address, Socket *socket)
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

unsigned char FEZLynx::ExtendedSockets::getPort(Socket::Pin pinNumber) {

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

	mainboard->panic("Port out of range");

	return NULL;
}

unsigned char FEZLynx::ExtendedSockets::getPin(Socket::Pin pinNumber) {
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

	mainboard->panic("Port out of range");

	return NULL;
}

void FEZLynx::ExtendedSockets::setIOMode(Socket::Pin pinNumber, IOState state, ResistorMode resistorMode) {
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
void FEZLynx::ExtendedSockets::setPWM(Socket::Pin pin, double frequency, double dutyCycle) {
	this->writeRegister(FEZLynx::ExtendedSockets::PWM_SELECT_REGISTER, (char)((pin % 8) + (this->getPort(pin) - 6) * 8));
	
	char period = (char)(93750 / frequency);

	this->writeRegister(FEZLynx::ExtendedSockets::PERIOD_REGISTER, period);
	this->writeRegister(FEZLynx::ExtendedSockets::PULSE_WIDTH_REGISTER, period * dutyCycle);
}

bool FEZLynx::ExtendedSockets::readDigital(Socket::Pin pin) {
	char b = this->readRegister(FEZLynx::ExtendedSockets::INPUT_PORT_0_REGISTER + this->getPort(pin));

	return b & this->getPin(pin);
}

void FEZLynx::ExtendedSockets::writeDigital(Socket::Pin pin, bool value) {
	char b = this->readRegister(FEZLynx::ExtendedSockets::OUTPUT_PORT_0_REGISTER + this->getPort(pin));

	if (value)
		b |= this->getPin(pin);
	else
		b &= ~this->getPin(pin);

	this->writeRegister(FEZLynx::ExtendedSockets::OUTPUT_PORT_0_REGISTER + this->getPort(pin), b);
}

double FEZLynx::ExtendedSockets::readAnalog(Socket::Pin pin) {
	mainboard->panic("Not supported");

	return 0.0;
}

void FEZLynx::ExtendedSockets::writeAnalog(Socket::Pin pin, double voltage) {
	mainboard->panic("Not supported");

}