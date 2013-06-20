#include "I2CBus.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

I2CBus::I2CBus(CPUPin sdaPin, CPUPin sclPin) : sda(sdaPin), scl(sclPin) {	

}

I2CBus::~I2CBus() {
	for (I2CDevice* current = (I2CDevice*)this->i2cDevices.start(); !this->i2cDevices.ended(); current = (I2CDevice*)this->i2cDevices.next())
		delete current;
}

I2CDevice* I2CBus::getI2CDevice(unsigned char address) {
	I2CDevice* device = new I2CDevice(this, address);
	this->i2cDevices.add(device);
	return device;
}
					
unsigned int I2CBus::write(const unsigned char* buffer, unsigned int count, unsigned char address, bool sendStop) {
	mainboard->panic(Exceptions::ERR_I2C_NOT_SUPPORTED);
}

unsigned int I2CBus::read(unsigned char* buffer, unsigned int count, unsigned char address, bool sendStop) {
	mainboard->panic(Exceptions::ERR_I2C_NOT_SUPPORTED);
}

bool I2CBus::writeRead(const unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength, unsigned int* numWritten, unsigned int* numRead, unsigned char address) {
	mainboard->panic(Exceptions::ERR_I2C_NOT_SUPPORTED);
}
