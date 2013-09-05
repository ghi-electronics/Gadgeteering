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

Mainboard* GHI::mainboard = NULL;

FEZLynx::FEZLynx() 
{
	GHI::mainboard = this;

	/////////////////////////////////////
	// FTDI Setup Code                 //
	/////////////////////////////////////

	//Set serial number to 3395969

	//Get Device Serial Numbers
	DWORD dwCount = 0;
	//Try to open the FT2232H device port and get the valid handle for subsequent access
	char SerialNumBuf[64];

	dwNumBytesRead = 0;
	dwNumBytesSent = 0;
	dwNumBytesToSend = 0;
	dwNumInputBuffer = 0;

	for(int i = 1; i < 4; i++)
	{
		ftStatus = FT_ListDevices((PVOID)i,& SerialNumBuf, FT_LIST_BY_INDEX|FT_OPEN_BY_SERIAL_NUMBER);

        if(ftStatus == FT_OK && (ftStatus = FT_Open(i,&Channels[i].device)) == FT_OK)
		{
			if(i == 1 || i == 0)
			{
				Channels[i].direction = 0xFB;
				Channels[i].data = 0x00;
			}

			ftStatus |= FT_ResetDevice(Channels[i].device); //Reset USB device

			//Purge USB receive buffer first by reading out all old data from FT2232H receive buffer
			do
			{
				ftStatus |= FT_GetQueueStatus(Channels[i].device, &dwNumInputBuffer); // Get the number of bytes in the FT2232H receive buffer
				
				if(dwNumInputBuffer == 0)
					break;

				if (dwNumInputBuffer <= 1024)
					FT_Read(Channels[i].device, &InputBuffer, dwNumInputBuffer, &dwNumBytesRead); //Read out the data from FT2232H receive buffer
				else
					FT_Read(Channels[i].device, &InputBuffer, 1024, &dwNumBytesRead);

			} while(dwNumInputBuffer > 0);

			//Set device configuration
			ftStatus |= FT_SetUSBParameters(Channels[i].device, 65536, 65535); //Set USB request transfer size
			ftStatus |= FT_SetChars(Channels[i].device, false, 0, false, 0); //Disable event and error characters
			ftStatus |= FT_SetTimeouts(Channels[i].device, 2000, 2000); //Sets the read and write timeouts in milliseconds for the FT2232H
			ftStatus |= FT_SetLatencyTimer(Channels[i].device, 0); //Set the latency timer
			ftStatus |= FT_SetBitMode(Channels[i].device, 0x0, 0x00); //Reset controller

			//Only channel A and B support MPSSE mode
			if(i == 0)
			{
				ftStatus |= FT_SetBitMode(Channels[i].device, Channels[i].direction, 0x02); //Enable MPSSE mode

				dwNumInputBuffer = 0;
				ftStatus |= FT_SetBaudRate(Channels[i].device, 115200);
			}
			else
			{
				if(i == 2)
					Channels[i].direction = 0x01;
				else
					Channels[i].direction = 0x00;

				Channels[i].data = 0x00;

				ftStatus |= FT_SetBitMode(Channels[i].device, Channels[i].direction, 0x01); //Enable Async BitBang Mode
				ftStatus |= FT_SetBaudRate(Channels[i].device, 115200);

				ftStatus = FT_Write(Channels[i].device, &Channels[i].data, 1, &dwNumBytesSent);
			}

			if (ftStatus != FT_OK)
			{
				this->panic(Exceptions::ERR_OUT_OF_SYNC);
				return;
			}

			//Allow USB stuff to complete..
			System::Sleep(50);

			for(int i_input = 0; i_input < 1024; i_input++)
				InputBuffer[i_input] = 0;
			
			dwNumBytesToSend = 0;

			//only channel A and B support MPSSE
			if(i == 0)
			{
				//////////////////////////////////////////////////////////////////
				// Below codes will synchronize the MPSSE interface by sending bad command 0xAA and checking if the echo command followed by 
				// bad command „AA‟ can be received, this will make sure the MPSSE interface enabled and synchronized successfully
				//////////////////////////////////////////////////////////////////
				OutputBuffer[dwNumBytesToSend++] = 0xAA; //Add BAD command 0xAA
				ftStatus = FT_Write(Channels[i].device, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send off the BAD commands
				dwNumBytesToSend = 0; //Clear output buffer

                do{
					ftStatus = FT_GetQueueStatus(Channels[i].device, &dwNumInputBuffer); // Get the number of bytes in the device input buffer
				}while ((dwNumInputBuffer == 0) && (ftStatus == FT_OK)); //or Timeout
			
				bool bCommandEchod = false;
				ftStatus = FT_Read(Channels[i].device, &InputBuffer, dwNumInputBuffer, &dwNumBytesRead); //Read out the data from input buffer
				for (dwCount = 0; dwCount < dwNumBytesRead - 1; dwCount++) //Check if Bad command and echo command received
				{
                    unsigned char FA = 0xFA;
                    unsigned char AA = 0xAA;

                    if ((InputBuffer[dwCount] == FA) && (InputBuffer[dwCount+1] == AA))
					{
						bCommandEchod = true;
						break;
					}
				}

				if (bCommandEchod == false) 
				{
					this->panic(Exceptions::ERR_OUT_OF_SYNC);
					return;
				}

				Channels[i].direction = 0xFB;
				Channels[i].data = 0x00;

				////////////////////////////////////////////////////////////////////
				//Configure the MPSSE settings for I2C communication with 24LC256
				//////////////////////////////////////////////////////////////////
				dwNumBytesToSend = 0;
				OutputBuffer[dwNumBytesToSend++] = '\x8A'; //Ensure disable clock divide by 5 for 60Mhz master clock
				OutputBuffer[dwNumBytesToSend++] = '\x97'; //Ensure turn off adaptive clocking

				//Comment back in for I2C
				//OutputBuffer[dwNumBytesToSend++] = '\x8D'; //Enable 3 phase data clock, used by I2C to allow data on both clock edges

				ftStatus = FT_Write(Channels[i].device, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send off the commands
				dwNumBytesToSend = 0; //Clear output buffer

				// The SK clock frequency can be worked out by below algorithm with divide by 5 set as off
				// SK frequency = 60MHz /((1 + [(1 +0xValueH*256) OR 0xValueL])*2)
				OutputBuffer[dwNumBytesToSend++] = '\x86'; //Command to set clock divisor
				OutputBuffer[dwNumBytesToSend++] = dwClockDivisor & '\xFF'; //Set 0xValueL of clock divisor
				OutputBuffer[dwNumBytesToSend++] = (dwClockDivisor >> 8) & '\xFF'; //Set 0xValueH of clock divisor

				ftStatus = FT_Write(Channels[i].device, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send off the commands
				dwNumBytesToSend = 0; //Clear output buffer

				System::Sleep(20); //Delay for a while

				//Turn off loop back in case
				OutputBuffer[dwNumBytesToSend++] = '\x85'; //Command to turn off loop back of TDI/TDO connection
				ftStatus = FT_Write(Channels[i].device, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send off the commands
				dwNumBytesToSend = 0; //Clear output buffer
				System::Sleep(30); //Delay for a while
			
				OutputBuffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output 
				OutputBuffer[dwNumBytesToSend++] = Channels[i].data; //Set SDA, SCL high, WP disabled by SK, DO at bit „1‟, GPIOL0 at bit „0‟
				OutputBuffer[dwNumBytesToSend++] = Channels[i].direction; //Set SK,DO,GPIOL0 pins as output with bit ‟, other pins as input with bit „‟
	
				ftStatus = FT_Write(Channels[i].device, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send off the commands
				dwNumBytesToSend = 0;
			}

		}
		else
			this->panic(Exceptions::ERR_OUT_OF_SYNC);
	}

	/////////////////////////////////////
	// Socket Setup Code               //
	/////////////////////////////////////
	
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

	socket = this->registerSocket(new Socket(6, Socket::Types::I));
	socket->pins[3] = Pins::P5_7;
	socket->pins[4] = Pins::NotConnected;
	socket->pins[5] = Pins::NotConnected;
	socket->pins[6] = Pins::P7_7;
	socket->pins[7] = Pins::NotConnected;
	socket->pins[8] = Pins::PB_1;
	socket->pins[9] = Pins::PB_0;

	socket = this->registerSocket(new Socket(7, Socket::Types::I));
	socket->pins[3] = Pins::P3_7;
	socket->pins[4] = Pins::NotConnected;
	socket->pins[5] = Pins::NotConnected;
	socket->pins[6] = Pins::P4_7;
	socket->pins[7] = Pins::NotConnected;
	socket->pins[8] = Pins::PB_1;
	socket->pins[9] = Pins::PB_0;

    ///////////////////////////////
    // Virtual Sockets
    ///////////////////////////////

    socket = this->registerSocket(new Socket(8, Socket::Types::Y | Socket::Types::P | Socket::Types::X));
    socket->pins[3] = Pins::P2_0;
    socket->pins[4] = Pins::P2_1;
    socket->pins[5] = Pins::P2_2;
    socket->pins[6] = Pins::P2_3;
    socket->pins[7] = Pins::P7_4;
    socket->pins[8] = Pins::P7_5;
    socket->pins[9] = Pins::P7_6;

    socket = this->registerSocket(new Socket(9, Socket::Types::Y | Socket::Types::P | Socket::Types::X));
    socket->pins[3] = Pins::P1_4;
    socket->pins[4] = Pins::P1_5;
    socket->pins[5] = Pins::P1_6;
    socket->pins[6] = Pins::P1_7;
    socket->pins[7] = Pins::P7_1;
    socket->pins[8] = Pins::P7_2;
    socket->pins[9] = Pins::P7_3;

    socket = this->registerSocket(new Socket(10, Socket::Types::Y | Socket::Types::X));
    socket->pins[3] = Pins::P3_0;
    socket->pins[4] = Pins::P3_1;
    socket->pins[5] = Pins::P3_2;
    socket->pins[6] = Pins::P3_3;
    socket->pins[7] = Pins::P3_4;
    socket->pins[8] = Pins::P3_5;
    socket->pins[9] = Pins::P3_6;

    socket = this->registerSocket(new Socket(11, Socket::Types::Y | Socket::Types::P | Socket::Types::X));
    socket->pins[3] = Pins::P0_0;
    socket->pins[4] = Pins::P0_1;
    socket->pins[5] = Pins::P0_2;
    socket->pins[6] = Pins::P0_3;
    socket->pins[7] = Pins::P6_0;
    socket->pins[8] = Pins::P6_1;
    socket->pins[9] = Pins::P6_2;

	socket = this->registerSocket(new Socket(12, Socket::Types::Y | Socket::Types::X));
    socket->pins[3] = Pins::P4_0;
    socket->pins[4] = Pins::P4_1;
    socket->pins[5] = Pins::P4_2;
    socket->pins[6] = Pins::P4_3;
    socket->pins[7] = Pins::P4_4;
    socket->pins[8] = Pins::P4_5;
    socket->pins[9] = Pins::P4_6;

    socket = this->registerSocket(new Socket(13, Socket::Types::Y | Socket::Types::P | Socket::Types::X));
    socket->pins[3] = Pins::P0_4;
    socket->pins[4] = Pins::P0_5;
    socket->pins[5] = Pins::P0_6;
    socket->pins[6] = Pins::P0_7;
    socket->pins[7] = Pins::P6_3;
    socket->pins[8] = Pins::P6_4;
    socket->pins[9] = Pins::P6_5;

    socket = this->registerSocket(new Socket(14, Socket::Types::Y | Socket::Types::X));
    socket->pins[3] = Pins::P4_0;
    socket->pins[4] = Pins::P4_1;
    socket->pins[5] = Pins::P4_2;
    socket->pins[6] = Pins::P4_3;
    socket->pins[7] = Pins::P4_4;
    socket->pins[8] = Pins::P4_5;
    socket->pins[9] = Pins::P4_6;

    socket = this->registerSocket(new Socket(15, Socket::Types::Y | Socket::Types::P | Socket::Types::X));
    socket->pins[3] = Pins::P5_0;
    socket->pins[4] = Pins::P5_1;
    socket->pins[5] = Pins::P5_2;
    socket->pins[6] = Pins::P5_3;
    socket->pins[7] = Pins::P5_4;
    socket->pins[8] = Pins::P5_5;
    socket->pins[9] = Pins::P5_6;

	/////////////////////////////
	// Extender Chip Setup     //
	/////////////////////////////

    this->Extender = new Modules::IO60P16(3);
	this->analogConverter = mainboard->getI2CBus(this->getSocket(3))->getI2CDevice(0x48);
}

FEZLynx::~FEZLynx() {

}

void FEZLynx::panic(unsigned char error, unsigned char specificError)
{
	std::cout <<  std::hex << error << " " << specificError << std::endl;

	throw error;
}

void FEZLynx::print(const char* toPrint) {
	std::cout << toPrint << std::endl;
}

void FEZLynx::print(int toPrint) {
	std::cout << toPrint << std::endl;
}

void FEZLynx::print(double toPrint) {
	std::cout << toPrint << std::endl;
}

unsigned char FEZLynx::GetChannelDirection(unsigned int channel)
{
	return Channels[channel].direction;
}

unsigned char FEZLynx::GetChannelMask(unsigned int channel)
{
	return Channels[channel].data;
}

bool FEZLynx::isVirtual(GHI::CPUPin pinNumber)
{
    int channel = GetChannel(pinNumber);

    if(channel > 3)
        return true;

	return false;
}

void FEZLynx::SetFTDIPins(int channel)
{
	switch(channel)
	{
		//Send MPSSE commands to channels 0 & 1
		case 0:
			dwNumBytesToSend = 0;

			OutputBuffer[dwNumBytesToSend++] = 0x80; //Command to set directions of lower 8 pins and force value on bits set as output
			OutputBuffer[dwNumBytesToSend++] = Channels[channel].data;
			OutputBuffer[dwNumBytesToSend++] = Channels[channel].direction;

			ftStatus = FT_Write(Channels[channel].device, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); //Send off the commands

			if(ftStatus != FT_OK)
				this->panic(0x00);

			ChannelDirectionChanged[channel] = false;

			break;

		//Send BitBang commands to channels 2 & 3
		case 1:
		case 2:
		case 3:
			ftStatus = FT_OK;

			if(ChannelDirectionChanged[channel])
			{
				ftStatus |= FT_SetBitMode(Channels[channel].device, Channels[channel].direction, 0x01); //Resync BitBang direction
				ChannelDirectionChanged[channel] = false;
			}

			ftStatus |= FT_Write(Channels[channel].device, &Channels[channel].data, 1, &dwNumBytesSent); //Set channel pin state

			if(ftStatus != FT_OK)
				this->panic(0x00);

			break;
	}
}

int FEZLynx::GetChannel(GHI::CPUPin pinNumber)
{
    int channel = 0;

    while(pinNumber > 8)
    {
        pinNumber -= 8;
        channel++;
    }

    if(channel > 12)
        this->panic(Exceptions::ERR_PORT_OUT_OF_RANGE);

    return channel;
}

unsigned char FEZLynx::GetChannelPin(GHI::CPUPin pinNumber)
{
    while(pinNumber > 8)
        pinNumber -= 8;

    return pinNumber;
}

void FEZLynx::setIOMode(GHI::CPUPin pinNumber, GHI::IOState state, GHI::ResistorMode resistorMode) {

	if(isVirtual(pinNumber))
	{
        int channel = GetChannel(pinNumber);
        int pin = GetChannelPin(pinNumber);
        CPUPin extendedPin = (((channel - 4) << 4) | (pin - 1));

        Extender->setIOMode(extendedPin, state, resistorMode);
	}

	else
	{
		if(state == GHI::IOStates::PWM)
			mainboard->panic(Exceptions::ERR_PWM_NOT_SUPPORTED);

		if(state == GHI::IOStates::DIGITAL_INPUT)
		{
			int channel = this->GetChannel(pinNumber);
            CPUPin pin = this->GetChannelPin(pinNumber);
			
            Channels[channel].direction &= ~(1 << (pin - 1));
			ChannelDirectionChanged[channel] = true;

			SetFTDIPins(channel);

			return;
		}

		else if(state == GHI::IOStates::DIGITAL_OUTPUT)
		{
			int channel = this->GetChannel(pinNumber);
            CPUPin pin = this->GetChannelPin(pinNumber);

            Channels[channel].direction |= (1 << (pin - 1));
			ChannelDirectionChanged[channel] = true;

			SetFTDIPins(channel);

			return;
		}

		mainboard->panic(Exceptions::ERR_IO_MODE_NOT_SUPPORTED);
	}
}

bool FEZLynx::readDigital(GHI::CPUPin pinNumber) {
	if(isVirtual(pinNumber))
    {
        int channel = GetChannel(pinNumber);
        int pin = GetChannelPin(pinNumber);
        CPUPin extendedPin = (((channel - 4) << 4) | (pin - 1));

        return Extender->readDigital(extendedPin);
	}
	else
	{
		DWORD sent = 0;
		DWORD dwBytesInQueue = 0;
		int timeout = 0;
		BYTE buffer[3];

		int channel = this->GetChannel(pinNumber);
        CPUPin pin = this->GetChannelPin(pinNumber);

		FT_STATUS status = 0;

		do
		{
			ftStatus |= FT_GetQueueStatus(Channels[channel].device, &dwNumInputBuffer); // Get the number of bytes in the FT2232H receive buffer
				
			if(dwNumInputBuffer == 0)
				break;
		
			if (dwNumInputBuffer <= 1024)
				FT_Read(Channels[channel].device, InputBuffer, dwNumInputBuffer, &dwNumBytesRead); //Read out the data from FT2232H receive buffer
			else
				FT_Read(Channels[channel].device, InputBuffer, 1024, &dwNumBytesRead);
		
		} while(dwNumInputBuffer > 0);

		if(channel == 0)
		{
			dwNumBytesToSend = 0;
			buffer[0] = 0x81;

			status = FT_Write(Channels[channel].device, buffer, 1, &sent); 
			dwNumBytesToSend = 0;

			while((dwBytesInQueue < 1) && (timeout < 500))
			{
				ftStatus |= FT_GetQueueStatus(Channels[channel].device, &dwBytesInQueue);
				System::Sleep(1);
				timeout++;
			}
		}

		status = FT_Read(Channels[channel].device, buffer, 1, &sent);   

        return (buffer[0] & (1 << (pin - 1))) == (1 << (pin - 1)) ? true : false;
	}

	return false;
}

void FEZLynx::writeDigital(GHI::CPUPin pinNumber, bool value) {
	if(isVirtual(pinNumber))
	{
        int channel = GetChannel(pinNumber);
        int pin = GetChannelPin(pinNumber);
        CPUPin extendedPin = (((channel - 4) << 4) | (pin - 1));

        Extender->writeDigital(extendedPin, value);
	}
	else
	{
		int channel = this->GetChannel(pinNumber);
		unsigned char pin = this->GetChannelPin(pinNumber);

		if(value)
            Channels[channel].data |= (1 << (pin - 1));
		else
            Channels[channel].data &= ~(1 << (pin - 1));

		this->SetFTDIPins(channel);
	}
}

double FEZLynx::readAnalog(GHI::CPUPin pinNumber) {
	unsigned char channel = 0x00;

	switch (pinNumber) {
		case Pins::PD_1: channel = 0; break;
		case Pins::PD_2: channel = 1; break;
		case FEZLynx::ANALOG_2: channel = 2; break;
		case Pins::PD_4: channel = 3; break;
		case Pins::PD_5: channel = 4; break;
		case FEZLynx::ANALOG_5: channel = 5; break;
		default: this->panic(Exceptions::ERR_READ_ANALOG_NOT_SUPPORTED);
	}

	if (pinNumber != FEZLynx::ANALOG_2 && pinNumber != FEZLynx::ANALOG_5)
		this->setIOMode(pinNumber, IOStates::DIGITAL_INPUT, ResistorModes::FLOATING);

	unsigned char command = 0x80 | 0x04; //CMD_SD_SE | CMD_PD_ON
	unsigned char read = 0x00;
	unsigned int a, b;

	command |= (unsigned char)((channel % 2 == 0 ? channel / 2 : (channel - 1) / 2 + 4) << 4);

	this->analogConverter->writeRead(&command, 1, &read, 1, &a, &b);

	return (double)read / 255.0 * 3.3;
}

double FEZLynx::readAnalogProportion(CPUPin pinNumber) {
	return this->readAnalog(pinNumber) / 3.3;
}

void FEZLynx::writeAnalog(GHI::CPUPin pinNumber, double voltage) {
	this->panic(Exceptions::ERR_WRITE_ANALOG_NOT_SUPPORTED);
}

void FEZLynx::writeAnalogProportion(CPUPin pinNumber, double proportion) {
	this->writeAnalog(pinNumber, proportion * 3.3);
}

void FEZLynx::setPWM(GHI::CPUPin pinNumber, double dutyCycle, double frequency) {

	//Only virtual sockets support PWM
	if(!isVirtual(pinNumber))
		this->panic(Exceptions::ERR_PWM_NOT_SUPPORTED);

    int channel = GetChannel(pinNumber);
    int pin = GetChannelPin(pinNumber);
    CPUPin extendedPin = (((channel - 4) << 4) | (pin - 1));

    Extender->setPWM(extendedPin,frequency,dutyCycle);
}

Interfaces::SerialDevice* FEZLynx::getSerialDevice(unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits, CPUPin txPin, CPUPin rxPin)
{
    for (SerialDevice* current = (SerialDevice*)this->serialDevices.start(); !this->serialDevices.ended(); current = (SerialDevice*)this->serialDevices.next())
        if (current->tx == txPin && current->rx == rxPin)
            return current;

    SerialDevice* bus = new FEZLynx::SerialDevice(txPin, rxPin, baudRate, parity, stopBits, dataBits, this->Channels[3].device);
    this->serialDevices.add(bus);

    return bus;
}

Interfaces::SPIBus* FEZLynx::getSPIBus(CPUPin miso, CPUPin mosi, CPUPin sck)
{
	for (SPIBus* current = (SPIBus*)this->spiBusses.start(); !this->spiBusses.ended(); current = (SPIBus*)this->spiBusses.next())
        if (current->mosi == mosi && current->miso == miso && current->sck == sck)
            return (GHI::Interfaces::SPIBus*)current;

	SPIBus* bus = new SPIBus(miso, mosi, sck, this->Channels[0].device);
    this->spiBusses.add(bus);

    return bus;
}

Interfaces::I2CBus *FEZLynx::getI2CBus(CPUPin sdaPin, CPUPin sclPin)
{
    for (I2CBus* current = (I2CBus*)this->i2cBusses.start(); !this->i2cBusses.ended(); current = (I2CBus*)this->i2cBusses.next())
        if (current->scl == sclPin && current->sda == sdaPin)
            return current;

    I2CBus* bus = new FEZLynx::I2CBus(sdaPin, sclPin, this->Channels[1].device);
    this->i2cBusses.add(bus);

    return bus;
}

#include "../LEDStrip/LEDStrip.h"
#include "../ButtonS6/ButtonS6.h"
#include "../Button/Button.h"
#include "../DisplayN18/DisplayN18.h"

int main() {
	FEZLynx board;
	//Modules::LEDStrip led(10);
	//Modules::ButtonS6 button(11);
	//Modules::Button button(11);
	Modules::DisplayN18 display(5);
	bool state = false;

	display.clear(0xBEEF);

	//while (true) {
	//	//if (state != button.isPressed()) {
	//	//	button.toggleLED();
	//	//	state = !state;
	//	//}
	//
	//	//led.set(1, button.isPressed(GHI::Modules::ButtonS6::Buttons::UP));
	//	//led.set(2, button.isPressed(GHI::Modules::ButtonS6::Buttons::LEFT));
	//	//led.set(3, button.isPressed(GHI::Modules::ButtonS6::Buttons::DOWN));
	//	//led.set(4, button.isPressed(GHI::Modules::ButtonS6::Buttons::RIGHT));
	//	//led.set(5, button.isPressed(GHI::Modules::ButtonS6::Buttons::LEFT_ARROW));
	//	//led.set(4, button.isPressed()); // button.isPressed(GHI::Modules::ButtonS6::Buttons::RIGHT_ARROW));
	//	//led.set(7, next);
	//
	//	//next = !next;
	//}

	return 0;
}