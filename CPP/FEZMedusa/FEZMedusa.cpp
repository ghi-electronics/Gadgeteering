#include "Arduino.h"
#include "FEZMedusa.h"
#include "../Gadgeteering/Types.hpp"
#include "../IO60P16/IO60P16.h"

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;

Mainboard* GHI::mainboard = NULL;

FEZMedusa::FEZMedusa() {
	mainboard = this;
	
	Socket* socket = this->registerSocket(new Socket(1, Socket::Types::I | Socket::Types::S | Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = 7;
	socket->pins[4] = 8;
	socket->pins[5] = 9;
	socket->pins[6] = 10;
	socket->pins[7] = 11;
	socket->pins[8] = 12;
	socket->pins[9] = 13;

	socket = this->registerSocket(new Socket(2, Socket::Types::I | Socket::Types::P | Socket::Types::U | Socket::Types::Y | Socket::Types::X));
	socket->pins[3] = 2;
	socket->pins[4] = 1;
	socket->pins[5] = 0;
	socket->pins[6] = 4;
	socket->pins[7] = 3;
	socket->pins[8] = 5;
	socket->pins[9] = 6;

	socket = this->registerSocket(new Socket(3, Socket::Types::A | Socket::Types::I | Socket::Types::X));
	socket->pins[3] = A0; //A0 = 14
	socket->pins[4] = A1; //A1 = 15
	socket->pins[5] = A2; //A2 = 16
	socket->pins[6] = A3; //A3 = 17
	socket->pins[8] = A4; //A4 = 18
	socket->pins[9] = A5; //A5 = 19

	socket = this->registerSocket(new Socket(5, Socket::Types::Y | Socket::Types::X | Socket::Types::P));
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

}
				
void FEZMedusa::panic(unsigned char error) {
	Serial.begin(9600);
	while (true)
		Serial.println((int)error);
}
				
void FEZMedusa::print(const char* toPrint) {
	Serial.print(toPrint);
}
				
void FEZMedusa::print(int toPrint) {
	Serial.print(toPrint);
}

void FEZMedusa::setIOMode(CPUPin pinNumber, IOState state, ResistorMode resistorMode) {
	if (!(pinNumber & FEZMedusa::EXTENDER_MASK)) {
		if (state == IOStates::PWM)
			mainboard->panic(Exceptions::ERR_PWM_NOT_SUPPORTED);
		else if (state == IOStates::DIGITAL_INPUT)
			::pinMode(pinNumber, resistorMode == ResistorModes::PULL_UP ? INPUT_PULLUP : INPUT);
		else
			::pinMode(pinNumber, OUTPUT);
	}
	else {
		this->extenderChip->setIOMode(pinNumber & ~FEZMedusa::EXTENDER_MASK, state, resistorMode);
	}
}

void FEZMedusa::setPWM(CPUPin pinNumber, double dutyCycle, double frequency) {
	!(pinNumber & FEZMedusa::EXTENDER_MASK) ? mainboard->panic(Exceptions::ERR_PWM_NOT_SUPPORTED) : this->extenderChip->setPWM(pinNumber & ~FEZMedusa::EXTENDER_MASK, dutyCycle, frequency);
}

bool FEZMedusa::readDigital(CPUPin pinNumber) {
	!(pinNumber & FEZMedusa::EXTENDER_MASK) ? ::digitalRead(pinNumber) == HIGH : this->extenderChip->readDigital(pinNumber & ~FEZMedusa::EXTENDER_MASK);
}

void FEZMedusa::writeDigital(CPUPin pinNumber, bool value) {
	!(pinNumber & FEZMedusa::EXTENDER_MASK) ? ::digitalWrite(pinNumber, value ? HIGH : LOW) : this->extenderChip->writeDigital(pinNumber & ~FEZMedusa::EXTENDER_MASK, value);
}

double FEZMedusa::readAnalog(CPUPin pinNumber) {
	if (!(pinNumber & FEZMedusa::EXTENDER_MASK)) 
	{
		return static_cast<double>(::analogRead(pinNumber)) / 1024 * 3.3;
	}
	else
	{
		mainboard->panic(Exceptions::ERR_READ_ANALOG_NOT_SUPPORTED);
		return 0.0;
	}
}

double FEZMedusa::readAnalogProportion(CPUPin pinNumber) {
	return this->readAnalog(pinNumber) / 3.3;
}

void FEZMedusa::writeAnalog(CPUPin pinNumber, double voltage) {
	!(pinNumber & FEZMedusa::EXTENDER_MASK) ? ::analogWrite(pinNumber, voltage * (1024 / 3.3)) : mainboard->panic(Exceptions::ERR_WRITE_ANALOG_NOT_SUPPORTED);
}

void FEZMedusa::writeAnalogProportion(CPUPin pinNumber, double voltage) {
	!(pinNumber & FEZMedusa::EXTENDER_MASK) ? ::analogWrite(pinNumber, voltage * 1024) : mainboard->panic(Exceptions::ERR_WRITE_ANALOG_NOT_SUPPORTED);
}

Interfaces::SPIBus* FEZMedusa::getNewSPIBus(CPUPin mosi, CPUPin miso, CPUPin sck) {
	return new FEZMedusa::SPIBus(mosi, miso, sck);
}

Interfaces::SPIBus* FEZMedusa::getNewSPIBus(Socket* socket, Socket::Pin mosiPinNumber, Socket::Pin misoPinNumber, Socket::Pin sckPinNumber) {
	return new FEZMedusa::SPIBus(socket->pins[mosiPinNumber], socket->pins[misoPinNumber], socket->pins[sckPinNumber]);
}

Interfaces::SerialDevice* FEZMedusa::getNewSerialDevice(CPUPin txPin, CPUPin rxPin, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits) {
	return new FEZMedusa::SerialDevice(txPin, rxPin, baudRate, parity, stopBits, dataBits);
}

Interfaces::SerialDevice* FEZMedusa::getNewSerialDevice(Socket* socket, Socket::Pin txPinNumber, Socket::Pin rxPinNumber, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits) {
	return new FEZMedusa::SerialDevice(socket->pins[txPinNumber], socket->pins[rxPinNumber], baudRate, parity, stopBits, dataBits);
}

Interfaces::I2CBus* FEZMedusa::getNewI2CBus(CPUPin sdaPin, CPUPin sclPin) {
	return new FEZMedusa::I2CBus(sdaPin, sclPin);
}

Interfaces::I2CBus* FEZMedusa::getNewI2CBus(Socket* socket, Socket::Pin sdaPinNumber, Socket::Pin sclPinNumber) {
	return new FEZMedusa::I2CBus(socket->pins[sdaPinNumber], socket->pins[sclPinNumber]);
}
