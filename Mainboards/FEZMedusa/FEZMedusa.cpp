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

#include "Arduino.h"
#include "../IO60P16/IO60P16.cpp"
#include "FEZMedusa.h"

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;

FEZMedusa::FEZMedusa() : FEZMedusaMini() {
	mainboard = this;

	Socket* socket = this->registerSocket(new Socket(5, Socket::Types::Y | Socket::Types::X | Socket::Types::P));
	socket->pins[3] = 0x00 | FEZMedusa::EXTENDER_MASK;
	socket->pins[4] = 0x01 | FEZMedusa::EXTENDER_MASK;
	socket->pins[5] = 0x02 | FEZMedusa::EXTENDER_MASK;
	socket->pins[6] = 0x03 | FEZMedusa::EXTENDER_MASK;
	socket->pins[7] = 0x60 | FEZMedusa::EXTENDER_MASK;
	socket->pins[8] = 0x61 | FEZMedusa::EXTENDER_MASK;
	socket->pins[9] = 0x62 | FEZMedusa::EXTENDER_MASK;

	socket = this->registerSocket(new Socket(6, Socket::Types::Y | Socket::Types::X | Socket::Types::P));
	socket->pins[3] = 0x04 | FEZMedusa::EXTENDER_MASK;
	socket->pins[4] = 0x05 | FEZMedusa::EXTENDER_MASK;
	socket->pins[5] = 0x06 | FEZMedusa::EXTENDER_MASK;
	socket->pins[6] = 0x07 | FEZMedusa::EXTENDER_MASK;
	socket->pins[7] = 0x63 | FEZMedusa::EXTENDER_MASK;
	socket->pins[8] = 0x64 | FEZMedusa::EXTENDER_MASK;
	socket->pins[9] = 0x65 | FEZMedusa::EXTENDER_MASK;

	socket = this->registerSocket(new Socket(7, Socket::Types::Y | Socket::Types::X | Socket::Types::P));
	socket->pins[3] = 0x10 | FEZMedusa::EXTENDER_MASK;
	socket->pins[4] = 0x11 | FEZMedusa::EXTENDER_MASK;
	socket->pins[5] = 0x12 | FEZMedusa::EXTENDER_MASK;
	socket->pins[6] = 0x13 | FEZMedusa::EXTENDER_MASK;
	socket->pins[7] = 0x66 | FEZMedusa::EXTENDER_MASK;
	socket->pins[8] = 0x67 | FEZMedusa::EXTENDER_MASK;
	socket->pins[9] = 0x70 | FEZMedusa::EXTENDER_MASK;
	
	socket = this->registerSocket(new Socket(8, Socket::Types::Y | Socket::Types::X | Socket::Types::P));
	socket->pins[3] = 0x14 | FEZMedusa::EXTENDER_MASK;
	socket->pins[4] = 0x15 | FEZMedusa::EXTENDER_MASK;
	socket->pins[5] = 0x16 | FEZMedusa::EXTENDER_MASK;
	socket->pins[6] = 0x17 | FEZMedusa::EXTENDER_MASK;
	socket->pins[7] = 0x71 | FEZMedusa::EXTENDER_MASK;
	socket->pins[8] = 0x72 | FEZMedusa::EXTENDER_MASK;
	socket->pins[9] = 0x73 | FEZMedusa::EXTENDER_MASK;

	socket = this->registerSocket(new Socket(9, Socket::Types::Y | Socket::Types::X | Socket::Types::P));
	socket->pins[3] = 0x20 | FEZMedusa::EXTENDER_MASK;
	socket->pins[4] = 0x21 | FEZMedusa::EXTENDER_MASK;
	socket->pins[5] = 0x22 | FEZMedusa::EXTENDER_MASK;
	socket->pins[6] = 0x23 | FEZMedusa::EXTENDER_MASK;
	socket->pins[7] = 0x74 | FEZMedusa::EXTENDER_MASK;
	socket->pins[8] = 0x75 | FEZMedusa::EXTENDER_MASK;
	socket->pins[9] = 0x76 | FEZMedusa::EXTENDER_MASK;

	socket = this->registerSocket(new Socket(10, Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = 0x30 | FEZMedusa::EXTENDER_MASK;
	socket->pins[4] = 0x31 | FEZMedusa::EXTENDER_MASK;
	socket->pins[5] = 0x32 | FEZMedusa::EXTENDER_MASK;
	socket->pins[6] = 0x33 | FEZMedusa::EXTENDER_MASK;
	socket->pins[7] = 0x34 | FEZMedusa::EXTENDER_MASK;
	socket->pins[8] = 0x35 | FEZMedusa::EXTENDER_MASK;
	socket->pins[9] = 0x36 | FEZMedusa::EXTENDER_MASK;

	socket = this->registerSocket(new Socket(11, Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = 0x40 | FEZMedusa::EXTENDER_MASK;
	socket->pins[4] = 0x41 | FEZMedusa::EXTENDER_MASK;
	socket->pins[5] = 0x42 | FEZMedusa::EXTENDER_MASK;
	socket->pins[6] = 0x43 | FEZMedusa::EXTENDER_MASK;
	socket->pins[7] = 0x44 | FEZMedusa::EXTENDER_MASK;
	socket->pins[8] = 0x45 | FEZMedusa::EXTENDER_MASK;
	socket->pins[9] = 0x46 | FEZMedusa::EXTENDER_MASK;

	socket = this->registerSocket(new Socket(12, Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = 0x50 | FEZMedusa::EXTENDER_MASK;
	socket->pins[4] = 0x51 | FEZMedusa::EXTENDER_MASK;
	socket->pins[5] = 0x52 | FEZMedusa::EXTENDER_MASK;
	socket->pins[6] = 0x53 | FEZMedusa::EXTENDER_MASK;
	socket->pins[7] = 0x54 | FEZMedusa::EXTENDER_MASK;
	socket->pins[8] = 0x55 | FEZMedusa::EXTENDER_MASK;
	socket->pins[9] = 0x56 | FEZMedusa::EXTENDER_MASK;

	this->extenderChip = new Modules::IO60P16(3);
}

FEZMedusa::~FEZMedusa() {
	delete this->extenderChip;
}

void FEZMedusa::setIOMode(CPUPin pinNumber, IOState state, ResistorMode resistorMode) {
	if (!(pinNumber & FEZMedusa::EXTENDER_MASK))
		FEZMedusaMini::setIOMode(pinNumber, state, resistorMode);
	else
		this->extenderChip->setIOMode(pinNumber & ~FEZMedusa::EXTENDER_MASK, state, resistorMode);
}

void FEZMedusa::setPWM(CPUPin pinNumber, double dutyCycle, double frequency) {
	!(pinNumber & FEZMedusa::EXTENDER_MASK) ? FEZMedusaMini::setPWM(pinNumber, dutyCycle, frequency) : this->extenderChip->setPWM(pinNumber & ~FEZMedusa::EXTENDER_MASK, dutyCycle, frequency);
}

bool FEZMedusa::readDigital(CPUPin pinNumber) {
	!(pinNumber & FEZMedusa::EXTENDER_MASK) ? FEZMedusaMini::readDigital(pinNumber) : this->extenderChip->readDigital(pinNumber & ~FEZMedusa::EXTENDER_MASK);
}

void FEZMedusa::writeDigital(CPUPin pinNumber, bool value) {
	!(pinNumber & FEZMedusa::EXTENDER_MASK) ? FEZMedusaMini::writeDigital(pinNumber, value) : this->extenderChip->writeDigital(pinNumber & ~FEZMedusa::EXTENDER_MASK, value);
}

double FEZMedusa::readAnalog(CPUPin pinNumber) {
	if (!(pinNumber & FEZMedusa::EXTENDER_MASK)) 
	{
		return FEZMedusaMini::readAnalog(pinNumber);
	}
	else
	{
		mainboard->panic(Exceptions::ERR_READ_ANALOG_NOT_SUPPORTED);
		return 0.0;
	}
}

void FEZMedusa::writeAnalog(CPUPin pinNumber, double voltage) {
	!(pinNumber & FEZMedusa::EXTENDER_MASK) ? FEZMedusaMini::writeAnalog(pinNumber, voltage) : mainboard->panic(Exceptions::ERR_WRITE_ANALOG_NOT_SUPPORTED);
}
