#include "I2CBus.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

I2CBus::I2CBus(CPUPin sdaPin, CPUPin sclPin) {	
	this->sda = sdaPin;
	this->scl = sclPin;
}

I2CBus::~I2CBus() {

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
