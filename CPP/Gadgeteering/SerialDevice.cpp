#include "SerialDevice.hpp"
#include "Mainboard.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

SerialDevice::SerialDevice(CPUPin txPin, CPUPin rxPin, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits) : tx(tx), rx(rx) {
	this->baudRate = baudRate;
	this->parity = parity;
	this->stopBits = stopBits;
	this->dataBits = dataBits;
}

SerialDevice::~SerialDevice() {

}

void SerialDevice::open() { };
void SerialDevice::close() { };
void SerialDevice::write(const unsigned char* buffer, unsigned int count) { mainboard->panic(Exceptions::ERR_SERIAL_NOT_SUPPORTED); };
void SerialDevice::write(const char* buffer, unsigned int count) { mainboard->panic(Exceptions::ERR_SERIAL_NOT_SUPPORTED); };
unsigned int SerialDevice::read(unsigned char* buffer, unsigned int count) { mainboard->panic(Exceptions::ERR_SERIAL_NOT_SUPPORTED); return 0; };