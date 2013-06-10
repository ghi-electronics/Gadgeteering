#include <SoftwareSerial.h>
#include "../Gadgeteering/SerialDevice.hpp"
#include "FEZMedusa.h"

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;

FEZMedusa::SerialDevice::SerialDevice(Socket* socket, int baudRate, int parity, int stopBits, int dataBits) : GHI::Interfaces::SerialDevice(socket, baudRate, parity, stopBits, dataBits) {
	this->port = new SoftwareSerial(socket->pins[SerialDevice::RX], socket->pins[SerialDevice::TX]);
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

void FEZMedusa::SerialDevice::write(const unsigned char* buffer, int count) {
	for (int i = 0; i < count; i++)
		this->port->write(buffer[i]);
}

void FEZMedusa::SerialDevice::write(const char* buffer, int count) {
	for (int i = 0; i < count; i++)
		this->port->write(buffer[i]);
}

void FEZMedusa::SerialDevice::read(char* buffer, int count) {
	for (int i = 0; i < count; i++)
		buffer[i] = this->port->read();
}