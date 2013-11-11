/*
Copyright 2013 Gadgeteering Electronics LLC

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

#include "ColorSense.h"

using namespace Gadgeteering;
using namespace Gadgeteering::Modules;
using namespace Gadgeteering::Interfaces;

ColorSense::ColorSense(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::X);

	this->LEDControl = new DigitalOutput(socket, Socket::Pins::Three, false);

	I2CBus* bus = mainboard->getI2CBus(socket->pins[5], socket->pins[4]);
	this->softwareI2C = bus->getI2CDevice(ColorSense::COLOR_ADDRESS);


	this->softwareI2C->writeRegister(0x80, 0x03);
}

ColorSense::~ColorSense() {
	delete this->LEDControl;
	delete this->softwareI2C;
}


void ColorSense::ToggleOnboardLED(bool LEDState) {
	this->LEDControl->write(LEDState);
}

ColorSense::ColorChannels ColorSense::ReadColorChannels() {
	ColorChannels returnData;

	unsigned char TransmitBuffer = 0x00;
	unsigned char read;
	unsigned int a, b;

	TransmitBuffer = 0x90; // Send COMMAND to access Green Color Channel register for chip
	softwareI2C->writeRead(&TransmitBuffer, 1, &read, 1, &a, &b);
	returnData.Green = read;
	TransmitBuffer = 0x91;
	softwareI2C->writeRead(&TransmitBuffer, 1, &read, 1, &a, &b);
	returnData.Green |= (unsigned int)(read) << 8;
	//returnData.Green = 256 * (uint)readWord(TransmitBuffer)[0] + returnData.Green;

	TransmitBuffer = 0x92; // Send COMMAND to access Red Color Channel register for chip
	softwareI2C->writeRead(&TransmitBuffer, 1, &read, 1, &a, &b);
	returnData.Red = read;
	TransmitBuffer = 0x93;
	softwareI2C->writeRead(&TransmitBuffer, 1, &read, 1, &a, &b);
	returnData.Red |= (unsigned int)(read) << 8;

	TransmitBuffer = 0x94; // Send COMMAND to access Blue Color Channel register for chip
	softwareI2C->writeRead(&TransmitBuffer, 1, &read, 1, &a, &b);
	returnData.Blue = read;
	TransmitBuffer = 0x95;
	softwareI2C->writeRead(&TransmitBuffer, 1, &read, 1, &a, &b);
	returnData.Blue |= (unsigned int)(read) << 8;

	TransmitBuffer = 0x96; // Send COMMAND to access Clear Channel register for chip
	softwareI2C->writeRead(&TransmitBuffer, 1, &read, 1, &a, &b);
	returnData.Clear = read;
	TransmitBuffer = 0x97;
	softwareI2C->writeRead(&TransmitBuffer, 1, &read, 1, &a, &b);
	returnData.Clear |= (unsigned int)(read) << 8;

	return returnData;
}