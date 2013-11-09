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

#ifndef _I2CBUS_H_
#define _I2CBUS_H_

#include "Socket.hpp"
#include "I2CDevice.hpp"
#include "Interfaces.hpp"
#include "List.hpp"

namespace GHI
{
	namespace Interfaces
	{
		class I2CBus
		{
			List i2cDevices;

			public:
				const CPUPin sda;
				const CPUPin scl;

				I2CBus(CPUPin sdaPin, CPUPin sclPin);
				virtual ~I2CBus();

				I2CDevice* getI2CDevice(unsigned char address);
					
				virtual unsigned int write(const unsigned char* buffer, unsigned int count, unsigned char address, bool sendStop = true);
				virtual unsigned int read(unsigned char* buffer, unsigned int count, unsigned char address, bool sendStop = true);
				virtual bool writeRead(const unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength, unsigned int* numWritten, unsigned int* numRead, unsigned char address);
		};
	}
}

#endif