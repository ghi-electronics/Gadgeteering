#ifndef _SPIBUS_H_
#define _SPIBUS_H_

#include "Socket.hpp"
#include "SPIDevice.hpp"
#include "Interfaces.hpp"

namespace GHI
{
	namespace Interfaces
	{
		class SPIBus
		{
			public:

				friend class GHI::Interfaces::SPIDevice;
				
				virtual ~SPIBus();

				//Clocks in one char and clocks out one char at the same time. If deselectChip is true, the CS line is set to logic low after the transmission, otherwise it remains logic high.
				virtual char writeReadByte(char toSend, GHI::Interfaces::SPIDevice::Configuration* configuration);

				//Clocks count bytes in and out at the same time to and from the receive and send buffer respectively.
				virtual void writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration);

				//Clocks sendCount bytes from sendBuffer out while ignoring the received bytes and then clocks receiveCount bytes into the receiveBuffer while clocking 0's out.
				virtual void writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, GHI::Interfaces::SPIDevice::Configuration* configuration);

				//Clocks count bytes out from the buffer while ignoring the bytes clocked in.
				virtual void write(char* buffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration);

				//Clocks count bytes in while clocking 0's out.
				virtual void read(char* buffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration);

			protected:
				SPIBus(Socket* socket);

				Socket* socket;
		};
	}
}
#endif