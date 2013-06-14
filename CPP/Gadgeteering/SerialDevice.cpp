/*#include "SerialDevice.hpp"
#include "Mainboard.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

SerialDevice::SerialDevice(CPUPin tx, CPUPin rx, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits) {
	this->tx = tx;
	this->rx = rx;
	this->baudRate = baudRate;
	this->parity = parity;
	this->stopBits = stopBits;
	this->dataBits = dataBits;
}

SerialDevice::~SerialDevice() {

}

void SerialDevice::open() { };
void SerialDevice::close() { };
void SerialDevice::write(const unsigned char* buffer, unsigned int count) { mainboard->panic(ERR_SERIAL_NOT_SUPPORTED); };
void SerialDevice::write(const char* buffer, unsigned int count) { mainboard->panic(ERR_SERIAL_NOT_SUPPORTED); };
void SerialDevice::read(unsigned char* buffer, unsigned int count) { mainboard->panic(ERR_SERIAL_NOT_SUPPORTED); };*/