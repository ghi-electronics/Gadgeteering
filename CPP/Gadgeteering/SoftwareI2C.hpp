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
			char address;
	
			void pullSCLLow();
			void pullSCLHigh();
			bool readSCL();
			void pullSDALow();
			void pullSDAHigh();
			bool readSDA();
	
			bool sendStartCondition(char address);
			void sendStopCondition();
			void waitForSCL();
	
			bool write(char data);
			char read(bool isLast);

			public:
				SoftwareI2C(char address, Socket* socket);

				bool writeRegisters(char startAddress, char count, char* data);
				bool readRegisters(char startAddress, char count, char* data);

				bool writeRegister(char location, char data);
				char readRegister(char location);
		};
	}
}

#endif
