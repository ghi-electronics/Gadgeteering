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

#include "IO60P16.h"

using namespace Gadgeteering;
using namespace Gadgeteering::Modules;
using namespace Gadgeteering::Interfaces;

IO60P16::IO60P16(unsigned char socketNumber, CPUPin sda, CPUPin scl) {
	Socket* socket = mainboard->getSocket(socketNumber);

	if (sda == 0 || scl == 0) {
		scl = socket->pins[4];
		sda = socket->pins[5];
		socket->ensureTypeIsSupported(Socket::Types::X);
	}

	this->io60Chip = mainboard->getI2CBus(sda, scl, false)->getI2CDevice(0x20);

	for(int i = 0; i < 8; i++)
	{
		this->outputPorts[i] = 0xFF;
		this->pinDirections[i] = 0x00;
		this->pwms[i] = 0x00;
		this->resistors[0][i] = 0xFF;
	}
	
	for(int i = 1; i < 4; i++)
		for(int j = 0; j < 8; j++)
			this->resistors[i][j] = 0x00;
}

IO60P16::~IO60P16() {
	delete this->io60Chip;
}

unsigned char IO60P16::getPort(CPUPin pin) {
	return pin >> 4;
}

unsigned char IO60P16::getMask(CPUPin pin) {
	return 1 << (pin & 0x0F);
}

void IO60P16::changeResistor(unsigned char port, unsigned char mask, unsigned char newResistor) {
	for (unsigned int i = 0; i < 4; i++)
		this->resistors[i][port] &= ~mask;

	unsigned int offset = 0;
	switch (newResistor) {
		case IO60P16::PIN_PULL_UP: offset = 0; break;
		case IO60P16::PIN_PULL_DOWN: offset = 1; break;
		case IO60P16::PIN_HIGH_IMPEDENCE: offset = 2; break;
		case IO60P16::PIN_STRONG_DRIVE: offset = 3; break;
	}

	this->resistors[offset][port] |= mask; 
	this->io60Chip->writeRegister(newResistor, this->resistors[offset][port]);
}

void IO60P16::setIOMode(CPUPin pinNumber, IOState state, ResistorMode resistorMode) {
	unsigned char mask = this->getMask(pinNumber);
	unsigned char port = this->getPort(pinNumber);
	unsigned char resistorRegister;

	if (state == IOStates::PWM_OUTPUT)	{
		if ((this->pwms[port] & mask) != 0x00)
			return; //already set as a PWM

		this->io60Chip->writeRegister(IO60P16::PORT_SELECT_REGISTER, port);

		this->pwms[port] |= mask;

		this->io60Chip->writeRegister(IO60P16::ENABLE_PWM_REGISTER, this->pwms[port]);
		
		this->writeDigital(pinNumber, true);

		this->io60Chip->writeRegister(IO60P16::PWM_SELECT_REGISTER, (unsigned char)((pinNumber % 8) + (port - 6) * 8));
		this->io60Chip->writeRegister(IO60P16::PWM_CONFIG, IO60P16::CLOCK_SOURCE); //93.75KHz clock
	}
	else {
		this->io60Chip->writeRegister(IO60P16::PORT_SELECT_REGISTER, port);

		if ((this->pwms[port] & mask) != 0x00) {
			this->pwms[port] &= ~mask;

			this->io60Chip->writeRegister(IO60P16::ENABLE_PWM_REGISTER, this->pwms[port]);
		}

		if (state == IOStates::DIGITAL_INPUT) {
			this->pinDirections[port] |= mask;

			resistorRegister = IO60P16::PIN_HIGH_IMPEDENCE;
			
			if (resistorMode == ResistorModes::PULL_DOWN)
				resistorRegister = IO60P16::PIN_PULL_DOWN;
			else if (resistorMode == ResistorModes::PULL_UP)
				resistorRegister = IO60P16::PIN_PULL_UP;
		}
		else {
			this->pinDirections[port] &= ~mask;
			
			resistorRegister = IO60P16::PIN_STRONG_DRIVE;
		}

		this->io60Chip->writeRegister(IO60P16::PIN_DIRECTION_REGISTER, this->pinDirections[port]);
			
		this->changeResistor(port, mask, resistorRegister);
	}
}

//We're using the 93.75KHz clock source because it gives a good resolution around the 1KHz frequency
//while still allowing the user to select frequencies such as 10KHz, but with reduced duty cycle
//resolution.
void IO60P16::setPWM(CPUPin pin, double frequency, double dutyCycle) {
	this->io60Chip->writeRegister(IO60P16::PWM_SELECT_REGISTER, (char)((pin % 8) + (this->getPort(pin) - 6) * 8));
	
	unsigned char period = (unsigned char)(93750 / frequency);

	this->io60Chip->writeRegister(IO60P16::PERIOD_REGISTER, period);
	this->io60Chip->writeRegister(IO60P16::PULSE_WIDTH_REGISTER, static_cast<unsigned char>(period * dutyCycle));
}

bool IO60P16::readDigital(CPUPin pin) {
	unsigned char b = this->io60Chip->readRegister(IO60P16::INPUT_PORT_0_REGISTER + this->getPort(pin));

	return (b & this->getMask(pin)) != 0;
}

void IO60P16::writeDigital(CPUPin pin, bool value) {
	unsigned char port = this->getPort(pin);
	unsigned char mask = (1 << (pin & 0x0F));

	if (value)
		this->outputPorts[port] |= mask;
	else
		this->outputPorts[port] &= ~mask;
	
	this->io60Chip->writeRegister(IO60P16::OUTPUT_PORT_0_REGISTER + port, this->outputPorts[port]);
}