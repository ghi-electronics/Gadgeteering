#include "I2CDevice.hpp"
#include "I2CBus.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

I2CDevice::I2CDevice(Interfaces::I2CBus* bus, unsigned char address) {
	this->bus = bus;
	this->address = address << 1;
}

I2CDevice::~I2CDevice() {

}

unsigned int I2CDevice::write(const unsigned char* buffer, unsigned int count, bool sendStop) {
	return this->bus->write(buffer, count, this->address, sendStop);
}

unsigned int I2CDevice::read(unsigned char* buffer, unsigned int count, bool sendStop) {
	return this->bus->read(buffer, count, this->address, sendStop);
}

bool I2CDevice::writeRead(const unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength, unsigned int* numWritten, unsigned int* numRead) {
	return this->bus->writeRead(writeBuffer, writeLength, readBuffer, readLength, numWritten, numRead, this->address);
}
				
bool I2CDevice::writeRegister(unsigned char address, unsigned char value) {
	unsigned char data[2] = {address, value};
	return this->write(data, 2, true);
}

unsigned char I2CDevice::readRegister(unsigned char address) {
	unsigned char value;
	unsigned int a, b;
	this->writeRead(&address, 1, &value, 1, &a, &b);
	return value;
}
