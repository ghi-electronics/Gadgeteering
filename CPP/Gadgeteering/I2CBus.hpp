#ifndef _I2CBUS_H_
#define _I2CBUS_H_

#include "Socket.hpp"
#include "I2CDevice.hpp"
#include "Interfaces.hpp"

namespace GHI
{
	namespace Interfaces
	{
		class I2CBus
		{
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
