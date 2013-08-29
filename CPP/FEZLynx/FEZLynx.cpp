﻿/*
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

// FEZLynx.cpp : Defines the entry point for the console application.
//

#include "FEZLynx.h"
#include "../Gadgeteering/System.hpp"
#include "../LED7R/LED7R.h"

#include <iostream>

using namespace std;

using namespace GHI;
using namespace GHI::Mainboards;

GHI::Mainboard* GHI::mainboard;

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

	for(int i = 0; i < 4; i++)
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
			ftStatus |= FT_SetLatencyTimer(Channels[i].device, 2); //Set the latency timer
			ftStatus |= FT_SetBitMode(Channels[i].device, 0x0, 0x00); //Reset controller

			//Only channel A and B support MPSSE mode
			if(i == 0 || i == 1)
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
			if(i == 0 || i == 1)
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
				OutputBuffer[dwNumBytesToSend++] = '\x8D'; //Enable 3 phase data clock, used by I2C to allow data on both clock edges

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
	socket->pins[5] = Pins::NotConnected; //VIN5
	socket->pins[6] = Pins::PD_6;
	socket->pins[7] = Pins::NotConnected;
	socket->pins[8] = Pins::PB_1;
	socket->pins[9] = Pins::PB_0;

	socket = this->registerSocket(new Socket(2, Socket::Types::I | Socket::Types::A));
	socket->pins[3] = Pins::PD_1;
	socket->pins[4] = Pins::PD_2;
	socket->pins[5] = Pins::NotConnected; //VIN2
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

    Extender = new Modules::IO60P16(3);
}

void FEZLynx::panic(unsigned char error)
{
	throw error;
}

void FEZLynx::panic(unsigned char error, unsigned char specificError)
{
	std::cout <<  std::hex << error << " " << specificError << endl;
	throw error;
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
		case 1:
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
				FT_Read(Channels[channel].device, &InputBuffer, dwNumInputBuffer, &dwNumBytesRead); //Read out the data from FT2232H receive buffer
			else
				FT_Read(Channels[channel].device, &InputBuffer, 1024, &dwNumBytesRead);

		} while(dwNumInputBuffer > 0);

		if(channel == 0 || channel == 1)
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
	//return static_cast<double>(::analogRead(this->pins[pinNumber])) / 1024 * 3.3;

	this->panic(Exceptions::ERR_NOT_IMPLEMENTED);

	return 0;
}

void FEZLynx::writeAnalog(GHI::CPUPin pinNumber, double voltage) {
	//::analogWrite(this->pins[pinNumber], voltage * (1024 / 3.3));

	this->panic(Exceptions::ERR_NOT_IMPLEMENTED);
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

void FEZLynx::setPWM(GHI::Socket* socket, GHI::Socket::Pin pin, double dutyCycle, double frequency)
{
    this->setPWM(socket->pins[pin], dutyCycle, frequency);
}

bool FEZLynx::readDigital(GHI::Socket* socket, GHI::Socket::Pin pin)
{
	return this->readDigital(socket->pins[pin]);
}

void FEZLynx::writeDigital(GHI::Socket* socket, GHI::Socket::Pin pin, bool value)
{
	this->writeDigital(socket->pins[pin], value);
}

double FEZLynx::readAnalog(GHI::Socket* socket, GHI::Socket::Pin pin)
{
	return this->readAnalog(socket->pins[pin]);
}

void FEZLynx::writeAnalog(GHI::Socket* socket, GHI::Socket::Pin pin, double voltage)
{
	this->writeAnalog(socket->pins[pin], voltage);
}

void FEZLynx::setIOMode(GHI::Socket* socket, GHI::Socket::Pin pin, GHI::IOState state, GHI::ResistorMode resistorMode)
{
	this->setIOMode(socket->pins[pin], state, resistorMode);
}

GHI::Interfaces::SPIBus* FEZLynx::getSPIBus(GHI::Socket* socket)
{
    for (SPIBus* current = (SPIBus*)this->spiBusses.start(); !this->spiBusses.ended(); current = (SPIBus*)this->spiBusses.next())
		if (current->mosi == socket->pins[7] && current->miso == socket->pins[8] && current->sck == socket->pins[9])
            return (GHI::Interfaces::SPIBus*)current;

	SPIBus* bus = new SPIBus(socket, Channels[0].device);
    this->spiBusses.add(bus);

    return (GHI::Interfaces::SPIBus*)bus;

    return NULL;
}

GHI::Interfaces::SPIBus* FEZLynx::getSPIBus(CPUPin miso, CPUPin mosi, CPUPin sck)
{
	for (SPIBus* current = (SPIBus*)this->spiBusses.start(); !this->spiBusses.ended(); current = (SPIBus*)this->spiBusses.next())
        if (current->mosi == mosi && current->miso == miso && current->sck == sck)
            return (GHI::Interfaces::SPIBus*)current;

	SPIBus* bus = new SPIBus(miso, mosi, sck, Channels[0].device);
    this->spiBusses.add(bus);

    return (GHI::Interfaces::SPIBus*)bus;

    return NULL;
}

GHI::Interfaces::SPIBus* FEZLynx::getSPIBus(Socket* socket, Socket::Pin mosiPinNumber, Socket::Pin misoPinNumber, Socket::Pin sckPinNumber)
{
	return this->getSPIBus(socket->pins[misoPinNumber], socket->pins[mosiPinNumber], socket->pins[sckPinNumber]);
}

GHI::Interfaces::SerialDevice* FEZLynx::getSerialDevice(GHI::Socket* socket, int baudRate, int parity, int stopBits, int dataBits)
{
    CPUPin txPin = socket->pins[4];
    CPUPin rxPin = socket->pins[5];

    for (SerialDevice* current = (SerialDevice*)this->serialDevices.start(); !this->serialDevices.ended(); current = (SerialDevice*)this->serialDevices.next())
        if (current->tx == txPin && current->rx == rxPin)
            return current;

    SerialDevice* bus = new FEZLynx::SerialDevice(txPin, rxPin, baudRate, parity, stopBits, dataBits);
	bus->SetChannel(Channels[3].device);
    this->serialDevices.add(bus);

    return bus;
}

Interfaces::I2CBus *FEZLynx::getI2CBus(CPUPin sdaPin, CPUPin sclPin)
{
    for (I2CBus* current = (I2CBus*)this->i2cBusses.start(); !this->i2cBusses.ended(); current = (I2CBus*)this->i2cBusses.next())
        if (current->scl == sclPin && current->sda == sdaPin)
            return current;

    I2CBus* bus = new FEZLynx::I2CBus(sdaPin, sclPin);
	bus->SetChannel(this->Channels[1].device);
    this->i2cBusses.add(bus);

    return bus;
}

Interfaces::I2CBus *FEZLynx::getI2CBus(Socket *socket, Socket::Pin sdaPinNumber, Socket::Pin sclPinNumber)
{
    return this->getI2CBus(socket->pins[sdaPinNumber], socket->pins[sclPinNumber]);
}

#include "../LED7R/LED7R.h"
#include "../Button/Button.h"
#include "../TouchC8/TouchC8.h"

int main() {
	FEZLynx board;
	Modules::LED7R led(10);
	Modules::Button button(11);
	Modules::TouchC8 touch(7);

	//while (true)

	while (true) {
		button.isPressed() ? led.turnOnLED(1) : led.turnOffLED(1);
		touch.IsButtonPressed(Modules::TouchC8::Buttons::A) ? led.turnOnLED(2) : led.turnOffLED(2);
		touch.IsButtonPressed(Modules::TouchC8::Buttons::B) ? led.turnOnLED(3) : led.turnOffLED(3);
		touch.IsButtonPressed(Modules::TouchC8::Buttons::C) ? led.turnOnLED(4) : led.turnOffLED(4);
		touch.IsProximityDetected() ? led.turnOnLED(5) : led.turnOffLED(5);

	}


	//Socket* socket = board.getSocket(3);
	//Interfaces::I2CDevice* i2c = socket->getI2CDevice(0x20);
	//
	//i2c->writeRegister(0x2A, 0xBE);
	//cout << hex << (int)i2c->readRegister(0x2A) << endl;

	return 0;
}