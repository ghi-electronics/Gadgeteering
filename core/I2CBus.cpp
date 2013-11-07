/*
Copyright 2013 GHI Electronics LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "I2CBus.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

I2CBus::I2CBus(CPUPin sdaPin, CPUPin sclPin) : sda(sdaPin), scl(sclPin) 
{	

}

I2CBus::~I2CBus() {
	for (I2CDevice* current = (I2CDevice*)this->i2cDevices.start(); !this->i2cDevices.ended(); current = (I2CDevice*)this->i2cDevices.next())
		delete current;
}

I2CDevice* I2CBus::getI2CDevice(unsigned char address) {
	I2CDevice* device = new I2CDevice(this, address);
	this->i2cDevices.addV(device);
	return device;
}
					
unsigned int I2CBus::write(const unsigned char* buffer, unsigned int count, unsigned char address, bool sendStop) {
	mainboard->panic(Exceptions::ERR_I2C_NOT_SUPPORTED, 0);

	return 0;
}

unsigned int I2CBus::read(unsigned char* buffer, unsigned int count, unsigned char address, bool sendStop) {
	mainboard->panic(Exceptions::ERR_I2C_NOT_SUPPORTED, 1);

	return 0;
}

bool I2CBus::writeRead(const unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength, unsigned int* numWritten, unsigned int* numRead, unsigned char address) {
	mainboard->panic(Exceptions::ERR_I2C_NOT_SUPPORTED, 2);

	return 0;
}
