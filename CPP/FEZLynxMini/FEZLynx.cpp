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

#include <iostream>

#include "FEZLynx.h"

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;

#define FTDI_CHANNEL(pin) ((pin - 1) / 8)
#define FTDI_PIN(pin) (((pin - 1) % 8) + 1)

Mainboard* GHI::mainboard = NULL;

FEZLynx::FEZLynx() 
{
	GHI::mainboard = this;
	
	FT_STATUS status = FT_OK;
	DWORD sent = 0, numDevices = 0;
	char serialNumberBuffer[64];
	unsigned char channel = 255;

	/*FT_DEVICE_LIST_INFO_NODE *infoList = new FT_DEVICE_LIST_INFO_NODE[10];
	status |= FT_GetDeviceInfoList(infoList, &numDevices);

	if(numDevices != 4)
		this->panic(Exceptions::ERR_MAINBOARD_ERROR);*/

	for(int i = 0; i < 4; i++)
	{
		status = FT_ListDevices((PVOID)i, &serialNumberBuffer, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER);

		//Sort devices by the serial number to ensure proper is selected
		if(serialNumberBuffer[strlen(serialNumberBuffer) - 1] == 'A')
			channel = 0;
		else if(serialNumberBuffer[strlen(serialNumberBuffer) - 1] == 'B')
			channel = 1;
		else if(serialNumberBuffer[strlen(serialNumberBuffer) - 1] == 'C')
			channel = 2;
		else if(serialNumberBuffer[strlen(serialNumberBuffer) - 1] == 'D')
			channel = 3;

		status |= FT_Open(i, &this->channels[channel].device);

		if (status != FT_OK)
			mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);

		this->initChannelPins(channel);
		this->setupChannel(channel);
		this->syncChannel(channel);
	}

	this->mapSockets();
	this->analogConverter = mainboard->getI2CBus(this->getSocket(3))->getI2CDevice(0x49);

	//delete [] infoList;
}

FEZLynx::~FEZLynx() 
{
	for(int i = 0; i < 4; i++)
	{
		//Return to default state before closing
		this->initChannelPins(i);
		this->setupChannel(i);

		FT_Close(this->channels[i].device);
	}
}

void FEZLynx::initChannelPins(unsigned char channel)
{
	this->channels[channel].isMPSSE = ((channel == 0 || channel == 1));

	if (channel == 1 ||channel == 0)
	{
		this->channels[channel].direction = 0xFB;
		this->channels[channel].value = 0x00;
	}
	else if (channel == 2)
	{
		this->channels[channel].direction = 0x00;
		this->channels[channel].value = 0x00;
	}
	else
	{
		this->channels[channel].direction = 0x00;
		this->channels[channel].value = 0x00;
	}
}

void FEZLynx::syncChannel(unsigned char channel)
{
	if(!this->channels[channel].isMPSSE)
		return;
	
	DWORD read, sent;
	FT_STATUS status = FT_OK;
	BYTE buffer[5];

	buffer[0] = 0xAA;
	status = FT_Write(this->channels[channel].device, buffer, 1, &sent);

	buffer[0] = 0x00;
	buffer[1] = 0x00;
	buffer[2] = 0x00;

	unsigned char loops = 0x00;

    while (true) 
	{
		status = FT_Read(this->channels[channel].device, buffer, 2, &read);

		if (buffer[0] == 0xFA && buffer[1] == 0xAA)
			break;

		if (++loops > 200)
			this->panic(Exceptions::ERR_OUT_OF_SYNC);
	}

	this->channels[channel].direction = 0xFB;
	this->channels[channel].value = 0x00;

	buffer[0] = 0x8A; //Disable clock divide by 5 for 60Mhz master clock
	buffer[1] = 0x97; //Turn off adaptive clocking
	//buffer[2] = 0x8D; //Enable 3 phase data clock
	status = FT_Write(this->channels[channel].device, buffer, 2, &sent);

	buffer[0] = 0x86; //Set clock divisor
	buffer[1] = FEZLynx::CLOCK_DIVISOR & 0xFF;
	buffer[2] = (FEZLynx::CLOCK_DIVISOR >> 8) & 0xFF;
	status = FT_Write(this->channels[channel].device, buffer, 3, &sent);

	System::Sleep(20);

	buffer[0] = 0x85; //Disable loop back of TDI/TDO connection
	status = FT_Write(this->channels[channel].device, buffer, 1, &sent);

	System::Sleep(30);
	
	if (status != FT_OK)
		this->panic(Exceptions::ERR_OUT_OF_SYNC);
}

void FEZLynx::setupChannel(unsigned char channel)
{
	FT_STATUS status = FT_OK;
	DWORD sent = 0;
	char *buffer = new char[3];

	status |= FT_ResetDevice(this->channels[channel].device);
	this->purgeChannel(channel);
	status |= FT_SetUSBParameters(this->channels[channel].device, 65536, 65535);
	status |= FT_SetChars(this->channels[channel].device, false, 0, false, 0);
	status |= FT_SetTimeouts(this->channels[channel].device, 2000, 2000);
	status |= FT_SetLatencyTimer(this->channels[channel].device, 2);
	status |= FT_SetBitMode(this->channels[channel].device, 0x0, 0x00);

	if (channel == 0 || channel == 1)
	{
		status |= FT_SetBitMode(this->channels[channel].device, this->channels[channel].direction, 0x02); //MPSSE mode

		buffer[0] = 0x80; //Set directions and values of lower 8 pins
		buffer[1] = this->channels[channel].value;
		buffer[2] = this->channels[channel].direction;
	
		status = FT_Write(this->channels[channel].device, buffer, 3, &sent);

		delete [] buffer;
	}
	else
	{
		status |= FT_SetBitMode(this->channels[channel].device, this->channels[channel].direction, 0x01); //Async BitBang Mode
		status |= FT_Write(this->channels[channel].device, &this->channels[channel].value, 1, &sent);
	}

	status |= FT_SetBaudRate(this->channels[channel].device, 19200);

	if (status != FT_OK)
		this->panic(Exceptions::ERR_OUT_OF_SYNC);

	System::Sleep(50);
}

void FEZLynx::purgeChannel(unsigned char channel)
{
	DWORD dwNumInputBuffer = 0, dwNumBytesRead = 0;
	char *InputBuffer = new char[1024];

	do
	{
		FT_STATUS ftStatus = FT_GetQueueStatus(channels[channel].device, &dwNumInputBuffer); // Get the number of bytes in the FT2232H receive buffer

		if(dwNumInputBuffer == 0)
			break;

		if (dwNumInputBuffer <= 1024)
			ftStatus |= FT_Read(channels[channel].device, &InputBuffer, dwNumInputBuffer, &dwNumBytesRead); //Read out the data from FT2232H receive buffer
		else
			ftStatus |= FT_Read(channels[channel].device, &InputBuffer, 1024, &dwNumBytesRead);

	} while(dwNumInputBuffer > 0);

	delete [] InputBuffer;
}

void FEZLynx::mapSockets()
{
	Socket* socket = this->registerSocket(new Socket(1, Socket::Types::I | Socket::Types::A));
	socket->pins[3] = Pins::PD_4;
	socket->pins[4] = Pins::PD_5;
	socket->pins[5] = FEZLynx::ANALOG_5; //VIN5
	socket->pins[6] = Pins::PD_6;
	socket->pins[7] = Pins::NotConnected;
	socket->pins[8] = Pins::PB_1;
	socket->pins[9] = Pins::PB_0;

	socket = this->registerSocket(new Socket(2, Socket::Types::I | Socket::Types::A));
	socket->pins[3] = Pins::PD_1;
	socket->pins[4] = Pins::PD_2;
	socket->pins[5] = FEZLynx::ANALOG_2; //VIN2
	socket->pins[6] = Pins::PD_3;
	socket->pins[7] = Pins::NotConnected;
	socket->pins[8] = Pins::PB_1;
	socket->pins[9] = Pins::PB_0;

	socket = this->registerSocket(new Socket(3, Socket::Types::K | Socket::Types::I | Socket::Types::X | Socket::Types::U));
	socket->pins[3] = Pins::PD_0;
	socket->pins[4] = Pins::PC_0;
	socket->pins[5] = Pins::PC_1;
	socket->pins[6] = Pins::PC_2;
	socket->pins[7] = Pins::PC_3;
	socket->pins[8] = Pins::PB_1;
	socket->pins[9] = Pins::PB_0;

	socket = this->registerSocket(new Socket(4, Socket::Types::S | Socket::Types::X));
	socket->pins[3] = Pins::PB_4;
	socket->pins[4] = Pins::PB_5;
	socket->pins[5] = Pins::PB_6;
	socket->pins[6] = Pins::PB_7;
	socket->pins[7] = Pins::PA_1;
	socket->pins[8] = Pins::PA_2;
	socket->pins[9] = Pins::PA_0;

	socket = this->registerSocket(new Socket(5, Socket::Types::S | Socket::Types::X));
	socket->pins[3] = Pins::PA_4;
	socket->pins[4] = Pins::PA_5;
	socket->pins[5] = Pins::PA_6;
	socket->pins[6] = Pins::PA_7;
	socket->pins[7] = Pins::PA_1;
	socket->pins[8] = Pins::PA_2;
    socket->pins[9] = Pins::PA_0;
}

void FEZLynx::panic(unsigned char error, unsigned char specificError)
{
	std::cout <<  std::hex << (int)error << " " << (int)specificError << std::endl;

	throw error;
}

void FEZLynx::print(const char* toPrint) 
{
	std::cout << toPrint << std::endl;
}

void FEZLynx::print(int toPrint) {
	std::cout << toPrint << std::endl;
}

void FEZLynx::print(double toPrint) 
{
	std::cout << toPrint << std::endl;
}

void FEZLynx::setValue(GHI::CPUPin pinNumber) 
{
    this->channels[FTDI_CHANNEL(pinNumber)].value |= (1 << (FTDI_PIN(pinNumber) - 1));
	this->sendPinStates(FTDI_CHANNEL(pinNumber));
}

void FEZLynx::clearValue(GHI::CPUPin pinNumber) 
{
    this->channels[FTDI_CHANNEL(pinNumber)].value &= ~(1 << (FTDI_PIN(pinNumber) - 1));
	this->sendPinStates(FTDI_CHANNEL(pinNumber));
}

void FEZLynx::setDirection(GHI::CPUPin pinNumber) 
{
    this->channels[FTDI_CHANNEL(pinNumber)].direction |= (1 << (FTDI_PIN(pinNumber) - 1));
	this->sendPinStates(FTDI_CHANNEL(pinNumber));
}

void FEZLynx::clearDirection(GHI::CPUPin pinNumber) 
{
    this->channels[FTDI_CHANNEL(pinNumber)].direction &= ~(1 << (FTDI_PIN(pinNumber) - 1));
	this->sendPinStates(FTDI_CHANNEL(pinNumber));
}

CPUPin FEZLynx::getExtenderPin(CPUPin pinNumber) 
{
    return (((FTDI_CHANNEL(pinNumber) - 4) << 4) | (FTDI_PIN(pinNumber) - 1));
}

bool FEZLynx::isVirtual(GHI::CPUPin pinNumber)
{
	return FTDI_CHANNEL(pinNumber) > 3;
}

void FEZLynx::sendPinStates(int channel)
{
	DWORD sent = 0;

	if (this->channels[channel].isMPSSE) 
	{
		BYTE buffer[3] = { 0x80, this->channels[channel].value, this->channels[channel].direction };

		if(FT_Write(this->channels[channel].device, buffer, 3, &sent) != FT_OK)
			this->panic(Exceptions::ERR_MAINBOARD_ERROR);

	}
	else 
	{
		FT_STATUS status = FT_SetBitMode(this->channels[channel].device, this->channels[channel].direction, 0x01);
		status |= FT_Write(this->channels[channel].device, &this->channels[channel].value, 1, &sent);

		if(status != FT_OK)
			this->panic(Exceptions::ERR_MAINBOARD_ERROR);
	}
}

void FEZLynx::setIOMode(GHI::CPUPin pinNumber, GHI::IOState state, GHI::ResistorMode resistorMode) 
{
	if(state == GHI::IOStates::DIGITAL_INPUT)
		this->clearDirection(pinNumber);
	else if(state == GHI::IOStates::DIGITAL_OUTPUT)
		this->setDirection(pinNumber);
	else
		mainboard->panic(Exceptions::ERR_IO_MODE_NOT_SUPPORTED);
}

bool FEZLynx::readDigital(GHI::CPUPin pinNumber) 
{
	DWORD sent = 0;
	FT_STATUS status = FT_OK;
	BYTE result = 0x00;
	int channel = FTDI_CHANNEL(pinNumber);

	//FT_Purge(this->channels[channel].device, FT_PURGE_RX);

	if(this->channels[channel].isMPSSE)
	{
		BYTE command = 0x81;

		status = FT_Write(this->channels[channel].device, &command, 1, &sent); 
	}

	status |= FT_Read(this->channels[channel].device, &result, 1, &sent);   

	if (status != FT_OK)
		mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);

    return (result & (1 << (FTDI_PIN(pinNumber) - 1))) > 0 ? true : false;
}

void FEZLynx::writeDigital(GHI::CPUPin pinNumber, bool value) 
{
	value ? this->setValue(pinNumber) : this->clearValue(pinNumber);
}

double FEZLynx::readAnalog(GHI::CPUPin pinNumber) 
{
	unsigned char channel = 0x00;

	switch (pinNumber) {
		case Pins::PD_1: channel = 0; break;
		case Pins::PD_2: channel = 1; break;
		case Pins::PA_3: channel = 2; break;
		case Pins::PD_4: channel = 3; break;
		case Pins::PD_5: channel = 4; break;
		case FEZLynx::ANALOG_5: channel = 5; break;

		default: this->panic(Exceptions::ERR_READ_ANALOG_NOT_SUPPORTED);
	}

	this->setIOMode(pinNumber, IOStates::DIGITAL_INPUT, ResistorModes::FLOATING);

	unsigned char command = 0x80 | 0x04; //CMD_SD_SE | CMD_PD_ON
	unsigned char read = 0x00;
	unsigned int a, b;

	command |= (unsigned char)((channel % 2 == 0 ? channel / 2 : (channel - 1) / 2 + 4) << 4);

	this->analogConverter->writeRead(&command, 1, &read, 1, &a, &b);

	return (double)read / 255.0 * 3.3;
}

double FEZLynx::readAnalogProportion(CPUPin pinNumber) 
{
	return this->readAnalog(pinNumber) / 3.3;
}

void FEZLynx::writeAnalog(GHI::CPUPin pinNumber, double voltage) 
{
	this->panic(Exceptions::ERR_WRITE_ANALOG_NOT_SUPPORTED);
}

void FEZLynx::writeAnalogProportion(CPUPin pinNumber, double proportion) 
{
	this->writeAnalog(pinNumber, proportion * 3.3);
}

void FEZLynx::setPWM(GHI::CPUPin pinNumber, double dutyCycle, double frequency) 
{
	this->panic(Exceptions::ERR_PWM_NOT_SUPPORTED);
}

Interfaces::SerialDevice* FEZLynx::getSerialDevice(unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits, CPUPin txPin, CPUPin rxPin)
{
    for (SerialDevice* current = (SerialDevice*)this->serialDevices.startV(); !this->serialDevices.ended(); current = (SerialDevice*)this->serialDevices.nextV())
        if (current->tx == txPin && current->rx == rxPin)
            return current;

    SerialDevice* bus = new FEZLynx::SerialDevice(txPin, rxPin, baudRate, parity, stopBits, dataBits, this->channels[3].device);
    this->serialDevices.addV(bus);
    return bus;
}

Interfaces::SPIBus* FEZLynx::getSPIBus(CPUPin miso, CPUPin mosi, CPUPin sck)
{
	for (SPIBus* current = (SPIBus*)this->spiBusses.startV(); !this->spiBusses.ended(); current = (SPIBus*)this->spiBusses.nextV())
        if (current->mosi == mosi && current->miso == miso && current->sck == sck)
            return (GHI::Interfaces::SPIBus*)current;

	SPIBus* bus = new SPIBus(miso, mosi, sck, this->channels[0].device);
    this->spiBusses.addV(bus);
    return bus;
}

Interfaces::I2CBus* FEZLynx::getI2CBus(CPUPin sdaPin, CPUPin sclPin)
{
    for (I2CBus* current = (I2CBus*)this->i2cBusses.startV(); !this->i2cBusses.ended(); current = (I2CBus*)this->i2cBusses.nextV())
        if (current->scl == sclPin && current->sda == sdaPin)
            return current;

    I2CBus* bus = new FEZLynx::I2CBus(sdaPin, sclPin, this->channels[1].device);
    this->i2cBusses.addV(bus);
    return bus;
}

#include "../LEDStrip/LEDStrip.h"
#include "../ButtonS6/ButtonS6.h"
#include "../Button/Button.h"
#include "../DisplayN18/DisplayN18.h"
#include "../FLASH/FLASH.h"

int main() {
	FEZLynx board;

	AnalogInput input(board.getSocket(1)->pins[3]);

	while(true)
	{
		std::cout << input.read() << std::endl;
		System::Sleep(100);
	}

	return 0;
}