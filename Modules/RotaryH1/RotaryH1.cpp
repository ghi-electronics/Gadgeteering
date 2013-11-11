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

#include "RotaryH1.h"

using namespace Gadgeteering;
using namespace Gadgeteering::Modules;
using namespace Gadgeteering::Interfaces;

RotaryH1::RotaryH1(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::Y);

	CS = new DigitalOutput(socket, Socket::Pins::Six, true);
	MISO = new DigitalInput(socket, Socket::Pins::Eight, ResistorModes::FLOATING);
	MOSI = new DigitalOutput(socket, Socket::Pins::Seven, false);
	CLOCK = new DigitalOutput(socket, Socket::Pins::Nine, false);

	Initialize();
}

RotaryH1::~RotaryH1() {
	delete CS;
	delete MISO;
	delete MOSI;
	delete CLOCK;
}

void RotaryH1::Initialize() {
	// Clear MDR0 register
	Write1Byte((unsigned char)Commands::LS7366_CLEAR | (unsigned char)Registers::LS7366_MDR0);

	// Clear MDR1 register
	Write1Byte((unsigned char)Commands::LS7366_CLEAR | (unsigned char)Registers::LS7366_MDR1);

	// Clear STR register
	Write1Byte((unsigned char)Commands::LS7366_CLEAR | (unsigned char)Registers::LS7366_STR);

	// Clear CNTR
	Write1Byte((unsigned char)Commands::LS7366_CLEAR | (unsigned char)Registers::LS7366_CNTR);

	// Clear ORT (write CNTR into OTR)
	Write1Byte((unsigned char)Commands::LS7366_LOAD | (unsigned char)Registers::LS7366_OTR);

	// Configure MDR0 register
	Write2Bytes((unsigned char)Commands::LS7366_WRITE        // write command
				| (unsigned char)Registers::LS7366_MDR0,       // to MDR0
				(unsigned char)MDR0Mode::LS7366_MDR0_QUAD1   // none quadrature mode
				| (unsigned char)MDR0Mode::LS7366_MDR0_FREER   // modulo-n counting 
				| (unsigned char)MDR0Mode::LS7366_MDR0_DIDX
				//| (unsigned char)MDR0Mode::LS7366_MDR0_LDOTR
				| (unsigned char)MDR0Mode::LS7366_MDR0_FFAC2);

	// Configure MDR1 register
	Write2Bytes((unsigned char)Commands::LS7366_WRITE        // write command
				| (unsigned char)Registers::LS7366_MDR1,       // to MDR1
				(unsigned char)MDR1Mode::LS7366_MDR1_2BYTE   // 2 unsigned char counter mode
				| (unsigned char)MDR1Mode::LS7366_MDR1_ENCNT);   // enable counting
}

unsigned char RotaryH1::Return1Byte(unsigned char reg) {
	LS7366_1B_wr[0] = reg;

	SoftwareSPI_WriteRead(LS7366_1B_wr, 1, LS7366_2B_rd, 2);
	return LS7366_2B_rd[1];
}

unsigned short RotaryH1::Return2Bytes(unsigned char reg) {
	int result = 0;
	LS7366_1B_wr[0] = reg;

	SoftwareSPI_WriteRead(LS7366_1B_wr, 1, LS7366_4B_rd, 4);
	result = (LS7366_4B_rd[1] * 256) + LS7366_4B_rd[2];

	return result;
}

void RotaryH1::Write1Byte(unsigned char reg) {
	LS7366_1B_wr[0] = reg;

	SoftwareSPI_WriteRead(LS7366_1B_wr, 1, NULL, 0);
}

void RotaryH1::Write2Bytes(unsigned char reg, unsigned char cmd) {
	LS7366_2B_wr[0] = reg;
	LS7366_2B_wr[1] = cmd;
	SoftwareSPI_WriteRead(LS7366_2B_wr, 2, NULL, 0);
}

void RotaryH1::SoftwareSPI_WriteRead(const unsigned char* write, unsigned int writeLength, unsigned char* read, unsigned int readLength) {
	int writeLen = writeLength;
	int readLen = 0;

	if (read != NULL) {
		readLen = readLength;

		for (int i = 0; i < readLen; i++) {
			read[i] = 0;
		}
	}

	int loopLen = (writeLen < readLen ? readLen : writeLen);

	unsigned char w = 0;

	CS->write(false);

	// per unsigned char
	for (int len = 0; len < loopLen; len++) {
		if (len < writeLen)
			w = write[len];

		unsigned char mask = 0x80;

		// per bit
		for (int i = 0; i < 8; i++) {
			CLOCK->write(false);

			if ((w & mask) == mask)
				MOSI->write(true);
			else
				MOSI->write(false);

			CLOCK->write(true);

			if (true == MISO->read())
			if (read != NULL)
				read[len] |= mask;

			mask >>= 1;
		}

		MOSI->write(false);
		CLOCK->write(false);
	}

	System::Sleep(2);
	CS->write(true);
}

long RotaryH1::ReadEncoders() {
	int retVal = Return2Bytes((unsigned char)Commands::LS7366_READ | (unsigned char)Registers::LS7366_CNTR);
	if ((ReadStatusReg() & 0x1)>0) // native number
	{
		retVal = ~retVal;
		retVal &= 0x7FFF;
		retVal *= (-1);
	}
	else {

	}

	return retVal;
}

unsigned char RotaryH1::ReadStatusReg() {
	unsigned char retVal = Return1Byte((unsigned char)((unsigned char)Commands::LS7366_READ | (unsigned char)Registers::LS7366_STR));//Return1Bytes((unsigned char)((unsigned char)Commands::LS7366_READ | (unsigned char)Registers::LS7366_STR));

	return retVal;
}

RotaryH1::Direction RotaryH1::ReadDirection() {
	unsigned char dir = (unsigned char)((ReadStatusReg() & 0x2) >> 1);
	return dir == 1 ? Directions::DOWN : Directions::UP;
}
