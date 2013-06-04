#include "SerialDevice.hpp"
#include <SoftwareSerial.h>

SerialDevice::SerialDevice(Socket* socket, int baudRate, int parity, int stopBits, int dataBits) {
	socket->ensureTypeIsSupported(Socket::Types::U);

	if (parity != Parity::NONE || stopBits != StopBits::NONE || dataBits != 8)
		mainboard->panic("Not supported");

	this->baudRate = baudRate;
	this->socket = socket;
	this->port = new SoftwareSerial(socket->pins[SerialDevice::RX], socket->pins[SerialDevice::TX]);
}

SerialDevice::~SerialDevice() {
	delete this->port;
}

void SerialDevice::open() {
	this->port->begin(this->baudRate);
}

void SerialDevice::close() {
	this->port->end();
}

void SerialDevice::write(const unsigned char* buffer, int count) {
	for (int i = 0; i < count; i++)
		this->port->write(buffer[i]);
}

void SerialDevice::write(const char* buffer, int count) {
	for (int i = 0; i < count; i++)
		this->port->write(buffer[i]);
}

void SerialDevice::read(byte* buffer, int count) {
	for (int i = 0; i < count; i++)
		buffer[i] = this->port->read();
}
