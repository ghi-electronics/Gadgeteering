#include "SerialDevice.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

SerialDevice::SerialDevice(CPUPin tx, CPUPin rx, int baudRate, int parity, int stopBits, int dataBits) {
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
void SerialDevice::write(const unsigned char* buffer, int count) { };
void SerialDevice::write(const char* buffer, int count) { };
void SerialDevice::read(unsigned char* buffer, int count) { };