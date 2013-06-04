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
			byte address;
	
			void pullSCLLow();
			void pullSCLHigh();
			bool readSCL();
			void pullSDALow();
			void pullSDAHigh();
			bool readSDA();
	
			bool sendStartCondition(byte address);
			void sendStopCondition();
			void waitForSCL();
	
			bool write(byte data);
			byte read(bool isLast);

			public:
				SoftwareI2C(byte address, Socket* socket);

				bool writeRegisters(byte startAddress, byte count, byte* data);
				bool readRegisters(byte startAddress, byte count, byte* data);

				bool writeRegister(byte location, byte data);
				byte readRegister(byte location);
		};
	}
}

#endif
