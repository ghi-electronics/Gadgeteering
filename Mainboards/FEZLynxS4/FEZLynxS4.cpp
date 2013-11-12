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

#include "FEZLynxS4.h"

#include <iostream>
#include <cstring>

using namespace Gadgeteering;
using namespace Gadgeteering::Interfaces;
using namespace Gadgeteering::Mainboards;

Mainboard* Gadgeteering::mainboard = NULL;

FEZLynxS4::FEZLynxS4() : Mainboard()
{
	this->channels[0].open("A");
	this->channels[1].open("B");
	this->channels[2].open("C");
	this->channels[3].open("D");

	this->channels[0].set_mode(ftdi_channel::modes::MPSSE);
	this->channels[1].set_mode(ftdi_channel::modes::MPSSE);
	this->channels[2].set_mode(ftdi_channel::modes::BITBANG);
	this->channels[3].set_mode(ftdi_channel::modes::BITBANG);

	Gadgeteering::mainboard = this;

    this->mapSockets();

	Socket* i2cSocket = this->getSocket(0);

	this->analogConverter = mainboard->getI2CBus(i2cSocket, Socket::Pins::Eight, Socket::Pins::Nine, false)->getI2CDevice(0x49);
	this->Extender = new Modules::IO60P16(0, i2cSocket->pins[8], i2cSocket->pins[9]);
}

FEZLynxS4::~FEZLynxS4()
{
    delete this->Extender;
    delete this->analogConverter;
}

void FEZLynxS4::mapSockets()
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

	socket = this->registerSocket(new Socket(0, Socket::Types::I));
	socket->pins[3] = Pins::PD_7;
	socket->pins[4] = Pins::NotConnected;
	socket->pins[5] = Pins::NotConnected;
	socket->pins[6] = Pins::NotConnected;
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

	socket = this->registerSocket(new Socket(4, Socket::Types::S | Socket::Types::X | Socket::Types::Y));
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

void FEZLynxS4::setDebugLED(bool state) {
	this->writeDigital(Pins::PA_3, state);
}

void FEZLynxS4::panic(unsigned char error, unsigned char specificError)
{
	std::cout << (int)error << " " << (int)specificError << std::endl;

	throw error;
}

void FEZLynxS4::print(const char* toPrint)
{
	std::cout << toPrint << std::endl;
}

void FEZLynxS4::print(int toPrint) {
	std::cout << toPrint << std::endl;
}

void FEZLynxS4::print(double toPrint)
{
	std::cout << toPrint << std::endl;
}

CPUPin FEZLynxS4::getExtenderPin(CPUPin pinNumber)
{
    return (((FTDI_CHANNEL(pinNumber) - 4) << 4) | (FTDI_PIN(pinNumber) - 1));
}

bool FEZLynxS4::isVirtual(Gadgeteering::CPUPin pinNumber)
{
	return FTDI_CHANNEL(pinNumber) > 3;
}

void FEZLynxS4::setIOMode(Gadgeteering::CPUPin pinNumber, Gadgeteering::IOState state, Gadgeteering::ResistorMode resistorMode)
{
	if(isVirtual(pinNumber))
	{
        int channel = FTDI_CHANNEL(pinNumber);
        int pin = FTDI_PIN(pinNumber);
        CPUPin extendedPin = (((channel - 4) << 4) | (pin - 1));

        Extender->setIOMode(extendedPin, state, resistorMode);

		return;
    }

	this->channels[FTDI_CHANNEL(pinNumber)].set_pin_direction(FTDI_PIN(pinNumber), state == IOStates::DIGITAL_INPUT ? ftdi_channel::pin_directions::INPUT : ftdi_channel::pin_directions::OUTPUT);
}

bool FEZLynxS4::readDigital(Gadgeteering::CPUPin pinNumber)
{
	if(isVirtual(pinNumber))
    {
        int channel = FTDI_CHANNEL(pinNumber);
        int pin = FTDI_PIN(pinNumber);
        CPUPin extendedPin = (((channel - 4) << 4) | (pin - 1));

        return Extender->readDigital(extendedPin);
    }

	return this->channels[FTDI_CHANNEL(pinNumber)].get_pin_state(FTDI_PIN(pinNumber));
}

void FEZLynxS4::writeDigital(Gadgeteering::CPUPin pinNumber, bool value)
{
	if(isVirtual(pinNumber))
	{
        int channel = FTDI_CHANNEL(pinNumber);
        int pin = FTDI_PIN(pinNumber);
        CPUPin extendedPin = (((channel - 4) << 4) | (pin - 1));

        Extender->writeDigital(extendedPin, value);

		return;
    }

	return this->channels[FTDI_CHANNEL(pinNumber)].set_pin_state(FTDI_PIN(pinNumber), value);
}

double FEZLynxS4::readAnalog(Gadgeteering::CPUPin pinNumber)
{
	unsigned char channel = 0x00;

	switch (pinNumber) {
		case Pins::PD_1: channel = 0; break;
		case Pins::PD_2: channel = 1; break;
		case Pins::PA_3: channel = 2; break;
		case Pins::PD_4: channel = 3; break;
		case Pins::PD_5: channel = 4; break;
		case FEZLynxS4::ANALOG_5: channel = 5; break;

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

double FEZLynxS4::readAnalogProportion(CPUPin pinNumber)
{
	return this->readAnalog(pinNumber) / 3.3;
}

void FEZLynxS4::writeAnalog(Gadgeteering::CPUPin pinNumber, double voltage)
{
	this->panic(Exceptions::ERR_WRITE_ANALOG_NOT_SUPPORTED);
}

void FEZLynxS4::writeAnalogProportion(CPUPin pinNumber, double proportion)
{
	this->writeAnalog(pinNumber, proportion * 3.3);
}

void FEZLynxS4::setPWM(Gadgeteering::CPUPin pinNumber, double dutyCycle, double frequency)
{
	this->panic(Exceptions::ERR_PWM_NOT_SUPPORTED);
}

Interfaces::SerialDevice* FEZLynxS4::getSerialDevice(unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits, CPUPin txPin, CPUPin rxPin)
{
    for (SerialDevice* current = (SerialDevice*)this->serialDevices.startV(); !this->serialDevices.ended(); current = (SerialDevice*)this->serialDevices.nextV())
        if (current->tx == txPin && current->rx == rxPin)
            return current;

	SerialDevice* bus = new FEZLynxS4::SerialDevice(txPin, rxPin, baudRate, parity, stopBits, dataBits, this->channels[3]);
    this->serialDevices.addV(bus);
    return bus;
}

Interfaces::SPIBus* FEZLynxS4::getSPIBus(CPUPin miso, CPUPin mosi, CPUPin sck)
{
	for (SPIBus* current = (SPIBus*)this->spiBusses.startV(); !this->spiBusses.ended(); current = (SPIBus*)this->spiBusses.nextV())
        if (current->mosi == mosi && current->miso == miso && current->sck == sck)
            return (Gadgeteering::Interfaces::SPIBus*)current;

    SPIBus* bus = new SPIBus(miso, mosi, sck, this->channels[0]);
    this->spiBusses.addV(bus);
    return bus;
}

Interfaces::I2CBus* FEZLynxS4::getI2CBus(CPUPin sdaPin, CPUPin sclPin)
{
    for (I2CBus* current = (I2CBus*)this->i2cBusses.startV(); !this->i2cBusses.ended(); current = (I2CBus*)this->i2cBusses.nextV())
        if (current->scl == sclPin && current->sda == sdaPin)
            return current;

		I2CBus *bus = new I2CBus(sdaPin, sclPin, this->channels[1]); 

        this->i2cBusses.addV(bus);
        return bus;
}
