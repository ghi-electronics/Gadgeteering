#ifndef _SOFTWAREI2C_H_
#define _SOFTWAREI2C_H_

#include "Types.hpp"
#include "Gadgeteering.h"

namespace GHI {
	namespace Interfaces {
		class SoftwareI2C {
			static const Socket::Pin SCL_PIN = Socket::Pins::Nine;
			static const Socket::Pin SDA_PIN = Socket::Pins::Eight;

			Socket* socket;
			unsigned char address;
			bool start;
	
			void clearSCL();
			bool readSCL();
			void clearSDA();
			bool readSDA();

			bool writeBit(bool bit);
			bool readBit();
	
			bool sendStartCondition();
			bool sendStopCondition();
			

			bool transmit(bool sendStart, bool sendStop, unsigned char data);
			unsigned char receive(bool sendAcknowledgeBit, bool sendStopCondition);
			
			public:
				SoftwareI2C(char address, Socket* socket);

				unsigned int writeBytes(unsigned char* data, unsigned int length);
				unsigned int readBytes(unsigned char* data, unsigned int length);
				bool writeRead(unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength, unsigned int* numWritten, unsigned int* numRead);
				
				bool writeRegister(unsigned char registerAddress, unsigned char value);
				unsigned char readRegister(unsigned char registerAddress);

		};
	}
}

#endif
