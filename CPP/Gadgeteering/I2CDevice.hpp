#ifndef _I2CDEVICE_H_
#define _I2CDEVICE_H_

#include "Types.hpp"
#include "Socket.hpp"

namespace GHI {
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
				unsigned char readRegister(unsigned char address);
		};
	}
}

#endif