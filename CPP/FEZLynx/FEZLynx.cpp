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

	DWORD read, sent;
	FT_STATUS status = FT_OK;
	BYTE buffer[5];
	unsigned char serialNumberBuffer[64];

	for(int i = 0; i < 4; i++)
	{
		status = FT_ListDevices((PVOID)i, &serialNumberBuffer, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER);
		status |= FT_Open(i, &this->channels[i].device);

		if (status != FT_OK)
			mainboard->panic(Exceptions::ERR_MAINBOARD_ERROR);

		this->channels[i].isMPSSE = i == 0;
		if(i == 1 || i == 0)
		{
			this->channels[i].direction = 0xFB;
			this->channels[i].value = 0x00;
		}
		else if (i == 2) {
			this->channels[i].direction = 0x01;
			this->channels[i].value = 0x00;
		}
		else {
			this->channels[i].direction = 0x00;
			this->channels[i].value = 0x00;
		}

		status |= FT_ResetDevice(this->channels[i].device);
		status |= FT_Purge(this->channels[i].device, FT_PURGE_RX);
		status |= FT_SetUSBParameters(this->channels[i].device, 65536, 65535);
		status |= FT_SetChars(this->channels[i].device, false, 0, false, 0);
		status |= FT_SetTimeouts(this->channels[i].device, 2000, 2000);
		status |= FT_SetLatencyTimer(this->channels[i].device, 0);
		status |= FT_SetBitMode(this->channels[i].device, 0x0, 0x00);
		status |= FT_SetBaudRate(this->channels[i].device, 115200);

		if (i == 0)
		{
			status |= FT_SetBitMode(this->channels[i].device, this->channels[i].direction, 0x02); //MPSSE mode
		}
		else
		{
			status |= FT_SetBitMode(this->channels[i].device, this->channels[i].direction, 0x01); //Async BitBang Mode
			status |= FT_Write(this->channels[i].device, &this->channels[i].value, 1, &sent);
		}

		if (status != FT_OK)
			this->panic(Exceptions::ERR_OUT_OF_SYNC);

		System::Sleep(50);

		if(i == 0)
		{
			buffer[0] = 0xAA;
			status = FT_Write(this->channels[i].device, buffer, 1, &sent);

			BYTE a = 0x00, b = 0x00, loops = 0x00;
            while (true) {
				status = FT_Read(this->channels[i].device, &b, 1, &read);

				if (a == 0xFA && b == 0xAA)
					break;

				a = b;

				if (++loops > 200)
					this->panic(Exceptions::ERR_OUT_OF_SYNC);
			}

			this->channels[i].direction = 0xFB;
			this->channels[i].value = 0x00;

			buffer[0] = 0x8A; //Disable clock divide by 5 for 60Mhz master clock
			buffer[1] = 0x97; //Turn off adaptive clocking
			//buffer[2] = 0x8D; //Enable 3 phase data clock
			status = FT_Write(this->channels[i].device, buffer, 2, &sent);

			buffer[0] = 0x86; //Set clock divisor
			buffer[1] = FEZLynx::CLOCK_DIVISOR & 0xFF;
			buffer[2] = (FEZLynx::CLOCK_DIVISOR >> 8) & 0xFF;
			status = FT_Write(this->channels[i].device, buffer, 3, &sent);

			System::Sleep(20);

			buffer[0] = 0x85; //Disable loop back of TDI/TDO connection
			status = FT_Write(this->channels[i].device, buffer, 1, &sent);

			System::Sleep(30);
			
			buffer[0] = 0x80; //Set directions and values of lower 8 pins
			buffer[1] = this->channels[i].value;
			buffer[2] = this->channels[i].direction;
	
			status = FT_Write(this->channels[i].device, buffer, 3, &sent);
		}
	}

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
    socket->pins[3] = Pins::P5_0;
    socket->pins[4] = Pins::P5_1;
    socket->pins[5] = Pins::P5_2;
    socket->pins[6] = Pins::P5_3;
    socket->pins[7] = Pins::P5_4;
    socket->pins[8] = Pins::P5_5;
    socket->pins[9] = Pins::P5_6;

    socket = this->registerSocket(new Socket(15, Socket::Types::Y | Socket::Types::P | Socket::Types::X));
    socket->pins[3] = Pins::P1_0;
    socket->pins[4] = Pins::P1_1;
    socket->pins[5] = Pins::P1_2;
    socket->pins[6] = Pins::P1_3;
    socket->pins[7] = Pins::P6_6;
    socket->pins[8] = Pins::P6_7;
    socket->pins[9] = Pins::P7_0;

    this->io60 = new Modules::IO60P16(3);
	this->analogConverter = mainboard->getI2CBus(this->getSocket(3))->getI2CDevice(0x48);
}

FEZLynx::~FEZLynx() {

}

void FEZLynx::panic(unsigned char error, unsigned char specificError)
{
	std::cout <<  std::hex << (int)error << " " << (int)specificError << std::endl;

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

void FEZLynx::setValue(GHI::CPUPin pinNumber) {
    this->channels[FTDI_CHANNEL(pinNumber)].value |= (1 << (FTDI_PIN(pinNumber) - 1));
	this->sendPinStates(FTDI_CHANNEL(pinNumber));
}

void FEZLynx::clearValue(GHI::CPUPin pinNumber) {
    this->channels[FTDI_CHANNEL(pinNumber)].value &= ~(1 << (FTDI_PIN(pinNumber) - 1));
	this->sendPinStates(FTDI_CHANNEL(pinNumber));
}

void FEZLynx::setDirection(GHI::CPUPin pinNumber) {
    this->channels[FTDI_CHANNEL(pinNumber)].direction |= (1 << (FTDI_PIN(pinNumber) - 1));
	this->sendPinStates(FTDI_CHANNEL(pinNumber));
}

void FEZLynx::clearDirection(GHI::CPUPin pinNumber) {
    this->channels[FTDI_CHANNEL(pinNumber)].direction &= ~(1 << (FTDI_PIN(pinNumber) - 1));
	this->sendPinStates(FTDI_CHANNEL(pinNumber));
}

CPUPin FEZLynx::getExtenderPin(CPUPin pinNumber) {
    return (((FTDI_CHANNEL(pinNumber) - 4) << 4) | (FTDI_PIN(pinNumber) - 1));
}

bool FEZLynx::isVirtual(GHI::CPUPin pinNumber)
{
	return FTDI_CHANNEL(pinNumber) > 3;
}

void FEZLynx::sendPinStates(int channel)
{
	DWORD sent = 0;

	if (this->channels[channel].isMPSSE) {
		BYTE buffer[3] = { 0x80, this->channels[channel].value, this->channels[channel].direction };

		if(FT_Write(this->channels[channel].device, buffer, 3, &sent) != FT_OK)
			this->panic(Exceptions::ERR_MAINBOARD_ERROR);

	}
	else {
		FT_STATUS status = FT_SetBitMode(this->channels[channel].device, this->channels[channel].direction, 0x01);
		status |= FT_Write(this->channels[channel].device, &this->channels[channel].value, 1, &sent);

		if(status != FT_OK)
			this->panic(Exceptions::ERR_MAINBOARD_ERROR);
	}
}

void FEZLynx::setIOMode(GHI::CPUPin pinNumber, GHI::IOState state, GHI::ResistorMode resistorMode) {
	if(isVirtual(pinNumber))
	{
        this->io60->setIOMode(this->getExtenderPin(pinNumber), state, resistorMode);
	}
	else
	{
		if(state == GHI::IOStates::DIGITAL_INPUT)
			this->clearDirection(pinNumber);
		else if(state == GHI::IOStates::DIGITAL_OUTPUT)
			this->setDirection(pinNumber);
		else
			mainboard->panic(Exceptions::ERR_IO_MODE_NOT_SUPPORTED);
	}
}

bool FEZLynx::readDigital(GHI::CPUPin pinNumber) {
	if(isVirtual(pinNumber))
        return this->io60->readDigital(this->getExtenderPin(pinNumber));

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

void FEZLynx::writeDigital(GHI::CPUPin pinNumber, bool value) {
	if(this->isVirtual(pinNumber))
        this->io60->writeDigital(this->getExtenderPin(pinNumber), value);
	else
		value ? this->setValue(pinNumber) : this->clearValue(pinNumber);
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
	if(!this->isVirtual(pinNumber))
		this->panic(Exceptions::ERR_PWM_NOT_SUPPORTED);

    this->io60->setPWM(this->getExtenderPin(pinNumber), frequency, dutyCycle);
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
#include "../FMRadio/FMRadio.h"

int main() {
	FEZLynx board;
	//Modules::LEDStrip led(10);
	//Modules::ButtonS6 button(11);
	//Modules::Button button(14);
	//Modules::DisplayN18 display(5);
	Modules::FMRadio radio(3);
	bool state = false;

	//display.clear(0xBEEF);

	//while (true) {
	//	if (state != button.isPressed()) {
	//		button.toggleLED();
	//		state = !state;
	//	}
	//
	//	std::cout << mainboard->readAnalog(mainboard->getSocket(1)->pins[3]) << std::endl;
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