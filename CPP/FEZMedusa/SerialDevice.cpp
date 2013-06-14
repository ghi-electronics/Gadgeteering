#include <SoftwareSerial.h>
#include "../Gadgeteering/SerialDevice.hpp"
#include "FEZMedusa.h"

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;

FEZMedusa::SerialDevice::SerialDevice(CPUPin tx, CPUPin rx, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits) : Interfaces::SerialDevice(tx, rx, baudRate, parity, stopBits, dataBits) {
	this->port = new SoftwareSerial(tx, rx);
}

FEZMedusa::SerialDevice::~SerialDevice() {
	delete this->port;
}

void FEZMedusa::SerialDevice::open() {
	this->port->begin(this->baudRate);
}

void FEZMedusa::SerialDevice::close() {
	this->port->end();
}

void FEZMedusa::SerialDevice::write(const unsigned char* buffer, unsigned int count) {
	for (unsigned int i = 0; i < count; i++)
		this->port->write(buffer[i]);
}

void FEZMedusa::SerialDevice::write(const char* buffer, unsigned int count) {
	for (unsigned int i = 0; i < count; i++)
		this->port->write(buffer[i]);
}

void FEZMedusa::SerialDevice::read(unsigned char* buffer, unsigned int count) {
	for (unsigned int i = 0; i < count; i++)
		buffer[i] = this->port->read();
}
