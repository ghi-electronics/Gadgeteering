#ifndef _SPI_H_
#define _SPI_H_

#include "Types.hpp"
#include "Socket.hpp"
#include "SPIDevice.hpp"

namespace GHI {
	namespace Interfaces {
		class DigitalOutput;
		class SPIBus;

		class SPIDevice {
			public:
				~SPIDevice();
		
				//Clocks in one char and clocks out one char at the same time. If deselectChip is true, the CS line is set to logic low after the transmission, otherwise it remains logic high.
				char writeReadByte(char toSend, bool deselectChip = false);

				//Clocks count bytes in and out at the same time to and from the receive and send buffer respectively.
				void writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count, bool deselectChip = false);

				//Clocks sendCount bytes from sendBuffer out while ignoring the received bytes and then clocks receiveCount bytes into the receiveBuffer while clocking 0's out.
				void writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, bool deselectChip = false);

				//Clocks count bytes out from the buffer while ignoring the bytes clocked in.
				void write(char* buffer, unsigned int count, bool deselectChip = false);

				//Clocks count bytes in while clocking 0's out.
				void read(char* buffer, unsigned int count, bool deselectChip = false);

			protected:
				SPIDevice(GHI::Interfaces::SPIBus *bus, GHI::Socket* socket, GHI::Socket::Pin chipSelectPin);

				SPIBus* bus;

				static const Socket::Pin CLOCK = Socket::Pins::Nine;
				static const Socket::Pin MISO = Socket::Pins::Eight;
				static const Socket::Pin MOSI = Socket::Pins::Seven;

				Socket* socket;
				DigitalOutput* chipSelect;
		};
	}
}

#endif