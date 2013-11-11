/*
Copyright 2013 Gadgeteering Electronics LLC

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

#ifndef _I2CDEVICE_H_
#define _I2CDEVICE_H_

#include "Types.h"
#include "Socket.h"

namespace Gadgeteering {
	namespace Interfaces {
		class I2CBus;

		class I2CDevice {
			protected:
				I2CBus* bus;
				unsigned char address;

			public:
				I2CDevice(Interfaces::I2CBus* bus, unsigned char address);
				~I2CDevice();

				unsigned int write(const unsigned char* buffer, unsigned int count, bool sendStop = true);
				unsigned int read(unsigned char* buffer, unsigned int count, bool sendStop = true);
				bool writeRead(const unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength, unsigned int* numWritten, unsigned int* numRead);
				
				bool writeRegister(unsigned char address, unsigned char value);
				bool writeRegisters(unsigned char startAddress, unsigned char* values, unsigned int count);
				unsigned char readRegister(unsigned char address);
				bool readRegisters(unsigned char startAddress, unsigned char* values, unsigned int count);
		};
	}
}

#endif