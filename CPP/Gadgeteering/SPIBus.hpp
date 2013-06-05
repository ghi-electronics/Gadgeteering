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

				class Configuration {
					public:
						//Whether or not the chip select pin is high when the chip is selected.
						bool chipSelectActiveState;

						//The amount of time (in milliseconds) that will elapse between when the device is selected and when the clock data transmission will start.
						unsigned int chipSelectSetupTime;

						//The amount of time (in milliseconds) that the chip-select port must remain in the active state before the device is unselected.
						unsigned int chipSelectHoldTime;

						//Whether or not the clock is high when the device is idle. True is high, false is low.
						bool clockIdleState;

						//The sampling clock edge. True if sampled on the rising edge, false on the falling edge.
						bool clockEdge;

						//Gets the clock rate, in KHz.
						unsigned int clockRate;

						Configuration(bool chipSelectActiveState, unsigned int chipSelectSetupTime, unsigned int chipSelectHoldTime, bool clockIdleState, bool clockEdge, unsigned int clockRate);
				};

				~SPIBus();

				//Clocks in one char and clocks out one char at the same time. If deselectChip is true, the CS line is set to logic low after the transmission, otherwise it remains logic high.
				virtual char writeReadByte(char toSend);

				//Clocks count bytes in and out at the same time to and from the receive and send buffer respectively.
				virtual void writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count);

				//Clocks sendCount bytes from sendBuffer out while ignoring the received bytes and then clocks receiveCount bytes into the receiveBuffer while clocking 0's out.
				virtual void writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount);

				//Clocks count bytes out from the buffer while ignoring the bytes clocked in.
				virtual void write(char* buffer, unsigned int count);

				//Clocks count bytes in while clocking 0's out.
				virtual void read(char* buffer, unsigned int count);

			protected:
				SPIBus(Socket* socket, GHI::Interfaces::SPIBus::Configuration* configuration);

				Configuration* configuration;
				Socket* socket;
		};
	}
}
#endif