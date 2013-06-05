#ifndef _SPIBUS_H_
#define _SPIBUS_H_

#include <Gadgeteering\Gadgeteering.h>

#include <iostream>

namespace GHI
{
	namespace Interfaces
	{
		class SPIBus
		{
			Socket::Pin MISO;
			Socket::Pin MOSI;
			Socket::Pin CLOCK;

			public:
				SPIBus(Socket::Pin miso, Socket::Pin mosi, Socket::Pin clock);

				//Clocks in one byte and clocks out one byte at the same time. If deselectChip is true, the CS line is set to logic low after the transmission, otherwise it remains logic high.
				byte writeReadByte(byte toSend, bool deselectChip = false);

				//Clocks count bytes in and out at the same time to and from the receive and send buffer respectively.
				void writeAndRead(byte* sendBuffer, byte* receiveBuffer, unsigned int count, bool deselectChip = false);

				//Clocks sendCount bytes from sendBuffer out while ignoring the received bytes and then clocks receiveCount bytes into the receiveBuffer while clocking 0's out.
				void writeThenRead(byte* sendBuffer, byte* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, bool deselectChip = false);

				//Clocks count bytes out from the buffer while ignoring the bytes clocked in.
				void write(byte* buffer, unsigned int count, bool deselectChip = false);

				//Clocks count bytes in while clocking 0's out.
				void read(byte* buffer, unsigned int count, bool deselectChip = false);

				void setClockHigh();
				void setClockLow();
				void setCSHigh();
				void setCSLow();
		};
	}
}
#endif