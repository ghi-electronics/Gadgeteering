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

#include <iostream>
#include <cstring>

#include "FEZLynx.h"

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;

Mainboard* GHI::mainboard = NULL;

FEZLynx::FEZLynx()
{
	GHI::mainboard = this;

    FT_STATUS status = FT_OK;
	char serialNumberBuffer[64];
    unsigned char channel = 255;
    int devices = 0;

    FTDI_Configuration config;
    config.Latency = 1;

	for(int i = 0; devices < 4; i++)
	{
        status = FT_ListDevices((PVOID)i, &serialNumberBuffer, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER);

		//Sort devices by the serial number to ensure proper channel is selected
        if(serialNumberBuffer[std::strlen(serialNumberBuffer) - 1] == 'A')
		{
            config.BitMode = FTDI_Configuration::BIT_MODE_MPSSE;
            this->m_devices[0] = new FTDI_Device(0xFB, 0x00, true, i);
            channel = 0;

            std::cout << "Found Channel A" << std::endl;
		}
        else if(serialNumberBuffer[std::strlen(serialNumberBuffer) - 1] == 'B')
		{
            config.BitMode = FTDI_Configuration::BIT_MODE_MPSSE;
            this->m_devices[1] = new FTDI_Device(0xFB, 0x00, true, i);
            channel = 1;

            std::cout << "Found Channel B" << std::endl;
		}
        else if(serialNumberBuffer[std::strlen(serialNumberBuffer) - 1] == 'C')
		{
            config.BitMode = FTDI_Configuration::BIT_MODE_ASYNC_BITBANG;
            this->m_devices[2] = new FTDI_Device(0xFF, 0x00, false, i);
            channel = 2;

            std::cout << "Found Channel C" << std::endl;
		}
        else if(serialNumberBuffer[std::strlen(serialNumberBuffer) - 1] == 'D')
		{
            config.BitMode = FTDI_Configuration::BIT_MODE_ASYNC_BITBANG;
            this->m_devices[3] = new FTDI_Device(0xFF, 0x00, false, i);
            channel = 3;

            std::cout << "Found Channel D" << std::endl;
		}

		if(channel != 255)
        {
            std::cout << "|--Setting up channel..." << std::endl;
            this->m_devices[channel]->SetupChannel(config);
            std::cout << "|--Syncing channel..." << std::endl << std::endl;
            this->m_devices[channel]->BadCommandSync();

            devices++;
            channel = 255;

            if(channel == 1 || channel == 0)
                this->m_devices[channel]->Open();
		}
    }

    this->mapSockets();
    this->analogConverter = mainboard->getI2CBus(this->getSocket(0),
                                                 Socket::Pins::Eight,
                                                 Socket::Pins::Nine,
                                                 true)->getI2CDevice(0x49);
    this->Extender = new Modules::IO60P16(0);
	//delete [] infoList;
}

FEZLynx::~FEZLynx()
{
    delete this->Extender;
    delete this->analogConverter;
}

void FEZLynx::mapSockets()
{
	Socket* socket = this->registerSocket(new Socket(1, Socket::Types::Y | Socket::Types::A));
	socket->pins[3] = Pins::PD_4; //VIN3
	socket->pins[4] = Pins::PD_5; //VIN4
	socket->pins[5] = Pins::PA_4; //VIN5
	socket->pins[6] = Pins::PD_6;
	socket->pins[7] = Pins::PA_5;
	socket->pins[8] = Pins::PA_6;
	socket->pins[9] = Pins::PA_7;

	socket = this->registerSocket(new Socket(2, Socket::Types::I | Socket::Types::A));
	socket->pins[3] = Pins::PD_1;
	socket->pins[4] = Pins::PD_2;
	socket->pins[5] = Pins::PA_1; //VIN2
	socket->pins[6] = Pins::PD_3;
	socket->pins[7] = Pins::NotConnected;
	socket->pins[8] = Pins::PB_1;
	socket->pins[9] = Pins::PB_0;

	socket = this->registerSocket(new Socket(0, Socket::Types::K | Socket::Types::I | Socket::Types::X | Socket::Types::U));
	socket->pins[3] = Pins::PD_7;
	socket->pins[4] = Pins::NotConnected;
	socket->pins[5] = Pins::NotConnected;
	socket->pins[6] = Pins::NotConnected;
	socket->pins[7] = Pins::NotConnected;
	socket->pins[8] = Pins::PB_1;
	socket->pins[9] = Pins::PB_0;

	socket = this->registerSocket(new Socket(3, Socket::Types::S | Socket::Types::X));
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

	socket = this->registerSocket(new Socket(11, Socket::Types::Y | Socket::Types::A));
    socket->pins[3] = Pins::P5_0;
	socket->pins[4] = Pins::P5_1;
    socket->pins[5] = Pins::P5_2;
    socket->pins[6] = Pins::P5_3;
    socket->pins[7] = Pins::P5_4;
    socket->pins[8] = Pins::P5_5;
    socket->pins[9] = Pins::P5_6;

    socket = this->registerSocket(new Socket(12, Socket::Types::Y | Socket::Types::A));
    socket->pins[3] = Pins::P4_0;
    socket->pins[4] = Pins::P4_1;
    socket->pins[5] = Pins::P4_2;
    socket->pins[6] = Pins::P4_3;
    socket->pins[7] = Pins::P4_4;
    socket->pins[8] = Pins::P4_5;
    socket->pins[9] = Pins::P4_6;

    socket = this->registerSocket(new Socket(13, Socket::Types::Y | Socket::Types::X));
    socket->pins[3] = Pins::P3_0;
    socket->pins[4] = Pins::P3_1;
    socket->pins[5] = Pins::P3_2;
    socket->pins[6] = Pins::P3_3;
    socket->pins[7] = Pins::P3_4;
    socket->pins[8] = Pins::P3_5;
    socket->pins[9] = Pins::P3_6;

    socket = this->registerSocket(new Socket(14, Socket::Types::Y | Socket::Types::P | Socket::Types::X));
    socket->pins[3] = Pins::P2_0;
    socket->pins[4] = Pins::P2_1;
    socket->pins[5] = Pins::P2_2;
    socket->pins[6] = Pins::P2_3;
    socket->pins[7] = Pins::P7_4;
    socket->pins[8] = Pins::P7_5;
    socket->pins[9] = Pins::P7_6;

	socket = this->registerSocket(new Socket(15, Socket::Types::Y | Socket::Types::P | Socket::Types::X));
    socket->pins[3] = Pins::P1_4;
    socket->pins[4] = Pins::P1_5;
    socket->pins[5] = Pins::P1_6;
    socket->pins[6] = Pins::P1_7;
    socket->pins[7] = Pins::P7_1;
    socket->pins[8] = Pins::P7_2;
    socket->pins[9] = Pins::P7_3;

    socket = this->registerSocket(new Socket(16, Socket::Types::Y | Socket::Types::P | Socket::Types::X));
    socket->pins[3] = Pins::P1_0;
    socket->pins[4] = Pins::P1_1;
    socket->pins[5] = Pins::P1_2;
    socket->pins[6] = Pins::P1_3;
    socket->pins[7] = Pins::P6_6;
    socket->pins[8] = Pins::P6_7;
    socket->pins[9] = Pins::P7_0;

    socket = this->registerSocket(new Socket(17, Socket::Types::Y | Socket::Types::P | Socket::Types::X));
    socket->pins[3] = Pins::P0_4;
    socket->pins[4] = Pins::P0_5;
    socket->pins[5] = Pins::P0_6;
    socket->pins[6] = Pins::P0_7;
    socket->pins[7] = Pins::P6_3;
    socket->pins[8] = Pins::P6_4;
    socket->pins[9] = Pins::P6_5;

    socket = this->registerSocket(new Socket(18, Socket::Types::Y | Socket::Types::P | Socket::Types::X));
    socket->pins[3] = Pins::P0_0;
    socket->pins[4] = Pins::P0_1;
    socket->pins[5] = Pins::P0_2;
    socket->pins[6] = Pins::P0_3;
    socket->pins[7] = Pins::P6_0;
    socket->pins[8] = Pins::P6_1;
    socket->pins[9] = Pins::P6_2;
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

CPUPin FEZLynx::getExtenderPin(CPUPin pinNumber)
{
    return (((FTDI_CHANNEL(pinNumber) - 4) << 4) | (FTDI_PIN(pinNumber) - 1));
}

bool FEZLynx::isVirtual(GHI::CPUPin pinNumber)
{
	return FTDI_CHANNEL(pinNumber) > 3;
}

void FEZLynx::setIOMode(GHI::CPUPin pinNumber, GHI::IOState state, GHI::ResistorMode resistorMode)
{
	if(isVirtual(pinNumber))
	{
        int channel = FTDI_CHANNEL(pinNumber);
        int pin = FTDI_PIN(pinNumber);
        CPUPin extendedPin = (((channel - 4) << 4) | (pin - 1));

        Extender->setIOMode(extendedPin, state, resistorMode);

		return;
    }

    if(state == GHI::IOStates::DIGITAL_INPUT)
        this->m_devices[FTDI_CHANNEL(pinNumber)]->ClearDirection(FTDI_PIN(pinNumber));
	else if(state == GHI::IOStates::DIGITAL_OUTPUT)
        this->m_devices[FTDI_CHANNEL(pinNumber)]->SetDirection(FTDI_PIN(pinNumber));
	else
		mainboard->panic(Exceptions::ERR_IO_MODE_NOT_SUPPORTED);

    if(FTDI_CHANNEL(pinNumber) == 0 || FTDI_CHANNEL(pinNumber) == 1)
	{
		this->m_devices[FTDI_CHANNEL(pinNumber)]->Open();
        this->m_devices[FTDI_CHANNEL(pinNumber)]->SetPinState();
	}
    else
    {
        this->m_devices[0]->Pause();
        this->m_devices[1]->Pause();

        this->m_devices[FTDI_CHANNEL(pinNumber)]->Open();
        this->m_devices[FTDI_CHANNEL(pinNumber)]->SetPinState();
        this->m_devices[FTDI_CHANNEL(pinNumber)]->Close();

        this->m_devices[0]->Resume();
        this->m_devices[1]->Resume();
    }
}

bool FEZLynx::readDigital(GHI::CPUPin pinNumber)
{
	if(isVirtual(pinNumber))
    {
        int channel = FTDI_CHANNEL(pinNumber);
        int pin = FTDI_PIN(pinNumber);
        CPUPin extendedPin = (((channel - 4) << 4) | (pin - 1));

        return Extender->readDigital(extendedPin);
    }

    unsigned char result = 0x00;

    if(FTDI_CHANNEL(pinNumber) == 0 || FTDI_CHANNEL(pinNumber) == 1)
	{
		this->m_devices[FTDI_CHANNEL(pinNumber)]->Open();
        result = this->m_devices[FTDI_CHANNEL(pinNumber)]->GetValue();
	}
    else
    {
        this->m_devices[0]->Pause();
        this->m_devices[1]->Pause();

        this->m_devices[FTDI_CHANNEL(pinNumber)]->Open();
        result = this->m_devices[FTDI_CHANNEL(pinNumber)]->GetValue();
        this->m_devices[FTDI_CHANNEL(pinNumber)]->Close();

        this->m_devices[0]->Resume();
        this->m_devices[1]->Resume();
    }

    return (result & (1 << (FTDI_PIN(pinNumber) - 1))) > 0 ? true : false;
}

void FEZLynx::writeDigital(GHI::CPUPin pinNumber, bool value)
{
	if(isVirtual(pinNumber))
	{
        int channel = FTDI_CHANNEL(pinNumber);
        int pin = FTDI_PIN(pinNumber);
        CPUPin extendedPin = (((channel - 4) << 4) | (pin - 1));

        Extender->writeDigital(extendedPin, value);

		return;
    }

    if(value)
        this->m_devices[FTDI_CHANNEL(pinNumber)]->SetValue(FTDI_PIN(pinNumber));
    else
        this->m_devices[FTDI_CHANNEL(pinNumber)]->ClearValue(FTDI_PIN(pinNumber));

    if(FTDI_CHANNEL(pinNumber) == 0 || FTDI_CHANNEL(pinNumber) == 1)
	{
		this->m_devices[FTDI_CHANNEL(pinNumber)]->Open();
        this->m_devices[FTDI_CHANNEL(pinNumber)]->SetPinState();
	}
    else
    {
        this->m_devices[0]->Pause();
        this->m_devices[1]->Pause();

        this->m_devices[FTDI_CHANNEL(pinNumber)]->Open();
        this->m_devices[FTDI_CHANNEL(pinNumber)]->SetPinState();
        this->m_devices[FTDI_CHANNEL(pinNumber)]->Close();

        this->m_devices[0]->Resume();
        this->m_devices[1]->Resume();
    }
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

    SerialDevice* bus = new FEZLynx::SerialDevice(txPin, rxPin, baudRate, parity, stopBits, dataBits, this->m_devices[3]);
    this->serialDevices.addV(bus);
    return bus;
}

Interfaces::SPIBus* FEZLynx::getSPIBus(CPUPin miso, CPUPin mosi, CPUPin sck)
{
	for (SPIBus* current = (SPIBus*)this->spiBusses.startV(); !this->spiBusses.ended(); current = (SPIBus*)this->spiBusses.nextV())
        if (current->mosi == mosi && current->miso == miso && current->sck == sck)
            return (GHI::Interfaces::SPIBus*)current;

    SPIBus* bus = new SPIBus(miso, mosi, sck, this->m_devices[0]); //this->channels[0].device);
    this->spiBusses.addV(bus);
    return bus;
}

Interfaces::I2CBus* FEZLynx::getI2CBus(CPUPin sdaPin, CPUPin sclPin, bool hardwareI2C)
{
    for (I2CBus* current = (I2CBus*)this->i2cBusses.startV(); !this->i2cBusses.ended(); current = (I2CBus*)this->i2cBusses.nextV())
        if (current->scl == sclPin && current->sda == sdaPin)
            return current;

    if(!hardwareI2C)
    {
        SoftwareI2CBus *bus = new SoftwareI2CBus(sdaPin, sclPin); //this->channels[1].device);

        this->i2cBusses.addV(bus);
        return bus;
    }
    else
    {
        I2CBus *bus = new I2CBus(sdaPin, sclPin, this->m_devices[1]); //this->channels[1].device);

        this->i2cBusses.addV(bus);
        return bus;
    }
}

#include "../ALFAT/ALFAT.h"
#include <iostream>
using namespace std;

int main()
{
	FEZLynx board;

    Modules::ALFAT fs(4);

	cout << fs.isStoragePresent(Modules::ALFAT::Drives::SD) << endl;

	return 0;
}
