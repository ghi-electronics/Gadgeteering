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

#include "ALFAT.h"
#include "string.h"

#define DECODE_HEX(c) ((c) <= '9' ? (c) - '0' : (c) - 'A' + 10)
#define NIBBLE_TO_HEX(n) ((n) >= 10 ? (n) - 10 + 'A' : (n) + '0')
#define HEX_TO_UCHAR(buffer) ((DECODE_HEX((buffer)[0]) << 8) + DECODE_HEX((buffer)[1]))

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

ALFAT::ALFAT(unsigned char socketNumber) {
	this->socket = mainboard->getSocket(socketNumber);
	this->socket->ensureTypeIsSupported(Socket::Types::S);
	
	this->handles = NULL;
	this->generateHandles();
	
	this->busyPin = new DigitalInput(this->socket, Socket::Pins::Four);
	this->resetPin = new DigitalOutput(this->socket, Socket::Pins::Three, false);

	this->reset();

	this->spi = socket->getSPIDevice(new SPIConfiguration(false, 1, 1, false, true, 6000), Socket::Pins::Six);

	//To consume the startup banner
	this->readFromDevice(NULL, 69);

	System::Sleep(1);
	
	//Success code is sent after the startup banner
	if (this->readResponseCode() != ResponseCodes::SUCCESS)
		mainboard->panic(Exceptions::ERR_MODULE_ERROR, 0);

	System::Sleep(1);
}

ALFAT::~ALFAT() {
	delete this->busyPin;
	delete this->resetPin;
	delete this->handles;
}

void ALFAT::reset() {
	this->resetPin->write(false);

	DigitalOutput mode1(socket, Socket::Pins::Six, true);
	DigitalOutput mode2(socket, Socket::Pins::Seven, true);

	System::Sleep(500);

	this->resetPin->write(true);

	System::Sleep(1000);
}

ALFAT::ResponseCode ALFAT::mount(Drive which) {
	this->drive = which;

	switch (which) {
		case Drives::SD: this->writeToDevice("I M:\n"); break;
		case Drives::USB0: this->writeToDevice("I U0:\n"); break;
		case Drives::USB1: this->writeToDevice("I U1:\n"); break;
	}

	System::Sleep(2);

	return this->readResponseCode();
}

void ALFAT::unmount() {
	this->generateHandles();
	this->drive = -1;
}

void ALFAT::generateHandles() {
	if (this->handles)
		delete this->handles;
	
	this->handles = new List();
	for (unsigned char i = 0; i < 16; i++)
        this->handles->push((void*)NIBBLE_TO_HEX(i));
}

bool ALFAT::isStoragePresent(ALFAT::Drive which) {
	this->writeToDevice("J\n");
	this->readResponseCode();

	unsigned char result[4];
	this->readFromDevice(result, 4);
	this->readResponseCode();

	unsigned char status = HEX_TO_UCHAR(result + 1);
	switch (which) {
		case Drives::SD: return (status & (1 << 0)) != 0;
		case Drives::USB0: return (status & (1 << 5)) != 0;
		case Drives::USB1: return (status & (1 << 6)) != 0;
	}

	return false;
}

bool ALFAT::isSDCardWriteProtected() {
	this->writeToDevice("J\n");
	this->readResponseCode();

	unsigned char result[4];
	this->readFromDevice(result, 4);
	this->readResponseCode();

	return (HEX_TO_UCHAR(result + 1) & (1 << 1)) != 0;
}

ALFAT::ResponseCode ALFAT::getVersion(unsigned char* major, unsigned char* minor, unsigned char* revision) {
	this->writeToDevice("V\n");

	unsigned char result[7];
	this->readFromDevice(result, 7);
	
	*major = result[1] - '0';
	*minor = result[3] - '0';
	*revision = result[5] - '0';

	return this->readResponseCode();
}

ALFAT::ResponseCode ALFAT::format(ALFAT::Drive which) {
	switch (which) {
		case Drives::SD: this->writeToDevice("Q CONFIRM FORMAT M:\n"); break;
		case Drives::USB0: this->writeToDevice("Q CONFIRM FORMAT U0:\n"); break;
		case Drives::USB1: this->writeToDevice("Q CONFIRM FORMAT U1:\n"); break;
	}

	this->readResponseCode();

	System::Sleep(25);

	return this->readResponseCode();
}

ALFAT::File* ALFAT::openFile(const char* path, ALFAT::File::Mode mode) {
	return new ALFAT::File(this, path, mode);
}

void ALFAT::sendWriteHeader(unsigned int length) {
	unsigned char frame[3] = {0x01, length & 0xFF, (length >> 8) & 0xFF};
	unsigned char read[3];

	while (true) {
		this->spi->writeAndRead(frame, read, 3, false);

		if (read[1] == 0x00 || read[2] == 0x00) {
			System::SleepMicro(50);
			continue;
		}
		else {
			break;
		}
	}
}

void ALFAT::writeToDevice(const char* command, unsigned int length, bool sendHeader, bool deselectAtEnd) {
	if (length == 0)
		length = strlen(command);

	if (sendHeader)
		this->sendWriteHeader(length);

	this->spi->write(reinterpret_cast<const unsigned char*>(command), length, deselectAtEnd);
}

void ALFAT::readFromDevice(unsigned char* buffer, unsigned int count) {
	unsigned char readFrame[3] = { 0x02, 0x00, 0x00 };
	unsigned char readResult[3];
	unsigned int available = 0;

	do {
		readFrame[1] = count & 0xFF;
		readFrame[2] = (count >> 8) & 0xFF;

		this->spi->writeAndRead(readFrame, readResult, 3, false);

		available = readResult[1] | (readResult[2] << 8);

		if (available == 0) {
			System::SleepMicro(50);
			continue;
		}
		else if (available > count) {
			available = count;
		}

		if (buffer) {
			this->spi->read(buffer, available, true);
		}
		else {
			for (unsigned int i = 0; i < available - 1; i++) 
				this->spi->writeReadByte(0x00, false);

			this->spi->writeReadByte(0x00, true);
		}

		count -= available;
		buffer += available;
	} while (count > 0);
}

char ALFAT::getHandle() {
	if (this->handles->getSize() == 0)
		return -1;

    unsigned char *handle = (unsigned char*)this->handles->pop();

    return (*handle & 0xFF);
}

void ALFAT::freeHandle(char handle) {
    this->handles->push(&handle);
}

ALFAT::ResponseCode ALFAT::readResponseCode() {
	System::SleepMicro(50);

	unsigned char result[4];
	this->readFromDevice(result, 4);

	return (result[0] == '!' && result[3] == '\n') ? HEX_TO_UCHAR(result + 1) : ResponseCodes::INVALID_RESPONSE;
}

void ALFAT::intToHex(unsigned int source, unsigned char* destination) {
	for (int i = 0; i < 8; i++)
		destination[i] = NIBBLE_TO_HEX((source >> (4 * (7 - i))) & 0xF);
}

unsigned int ALFAT::hexToInt(const unsigned char* source) {
	unsigned int result = 0;

	for (int i = 0; i < 4; i++)
		result |= (HEX_TO_UCHAR(source + i * 2) << (3 - i));

	return result;
}



ALFAT::File::File(ALFAT* parent, const char* path, ALFAT::File::Mode mode) : alfat(parent) {
	this->handle = this->alfat->getHandle();

	unsigned int pathLength = strlen(path);

	if (this->alfat->drive == ALFAT::Drives::SD) {
		char command[7] = { 'O', ' ', this->handle, (char)mode, '>', 'M', ':' };
		this->alfat->sendWriteHeader(7 + pathLength + 1);
		this->alfat->writeToDevice(command, 7, false, false);
	}
	else {
		char command[8] = { 'O', ' ', this->handle, (char)mode, '>', 'U', this->alfat->drive == ALFAT::Drives::USB0 ? '0' : '1', ':' };
		this->alfat->sendWriteHeader(8 + pathLength + 1);
		this->alfat->writeToDevice(command, 8, false, false);
	}

	this->alfat->writeToDevice(path, pathLength, false, false);

	char lf = '\n';
	this->alfat->writeToDevice(&lf, 1, false, true);

	this->alfat->readResponseCode();
}

ALFAT::File::~File() {
	this->alfat->freeHandle(this->handle);
}

ALFAT::ResponseCode ALFAT::File::rename(const char* newName) {
	return ResponseCodes::SUCCESS;
}

ALFAT::ResponseCode ALFAT::File::remove() {
	return ResponseCodes::SUCCESS;
}

ALFAT::ResponseCode ALFAT::File::close() {
	unsigned char frame[4] = { 'C', ' ', this->handle, '\n' };

	this->alfat->writeToDevice(reinterpret_cast<char*>(frame), 4);

	return this->alfat->readResponseCode();
}

ALFAT::ResponseCode ALFAT::File::flush() {
	unsigned char frame[4] = { 'F', ' ', this->handle, '\n' };

	this->alfat->writeToDevice(reinterpret_cast<char*>(frame), 4);

	return this->alfat->readResponseCode();
}

ALFAT::ResponseCode ALFAT::File::seek(unsigned int position) {
	return ResponseCodes::SUCCESS;
}

ALFAT::ResponseCode ALFAT::File::write(const unsigned char* buffer, unsigned int count, unsigned int* actualWritten) {
	if (count == 0) {
		if (actualWritten)
			*actualWritten = 0;
		return ResponseCodes::SUCCESS;
	}

	unsigned char frame[13] = { 'W', ' ', this->handle, '>', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '\n' };
	ALFAT::intToHex(count, frame + 4);
	
	this->alfat->writeToDevice(reinterpret_cast<char*>(frame), 13);
	
	this->alfat->readResponseCode();
	
	this->alfat->writeToDevice(reinterpret_cast<const char*>(buffer), count, true, true);
	
	unsigned char remainder[10];
	this->alfat->readFromDevice(remainder, 10);
	if (actualWritten)
		*actualWritten = ALFAT::hexToInt(remainder + 1);

	return this->alfat->readResponseCode();
}

ALFAT::ResponseCode ALFAT::File::read(unsigned char* buffer, unsigned int count, unsigned int* actualRead) {
	if (count == 0) {
		if (actualRead)
			*actualRead = 0;
		return ResponseCodes::SUCCESS;
	}

	unsigned char frame[14] = { 'R', ' ', this->handle, 0x00, '>', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '\n' };
	ALFAT::intToHex(count, frame + 5);

	this->alfat->writeToDevice(reinterpret_cast<char*>(frame), 14);

	this->alfat->readResponseCode();

	this->alfat->readFromDevice(buffer, count);

	unsigned char remainder[10];
	this->alfat->readFromDevice(remainder, 10);
	if (actualRead)
		*actualRead = ALFAT::hexToInt(remainder + 1);

	return this->alfat->readResponseCode();
}
