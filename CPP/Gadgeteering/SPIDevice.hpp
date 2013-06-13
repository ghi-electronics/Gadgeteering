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
				
				SPIDevice(Interfaces::SPIBus* bus, CPUPin chipSelectPin, Configuration* configuration);
				SPIDevice(Interfaces::SPIBus* bus, Socket* socket, Socket::Pin chipSelectPinNumber, Configuration* configuration);
				~SPIDevice();

				//Clocks in one char and clocks out one char at the same time. If deselectChip is true, the CS line is set to logic low after the transmission, otherwise it remains logic high.
				unsigned char writeReadByte(unsigned char toSend, bool deselectChip = false);

				//Clocks count bytes in and out at the same time to and from the receive and send buffer respectively.
				void writeAndRead(unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, bool deselectChip = false);

				//Clocks sendCount bytes from sendBuffer out while ignoring the received bytes and then clocks receiveCount bytes into the receiveBuffer while clocking 0's out.
				void writeThenRead(unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, bool deselectChip = false);

				//Clocks count bytes out from the buffer while ignoring the bytes clocked in.
				void write(unsigned char* buffer, unsigned int count, bool deselectChip = false);

				//Clocks count bytes in while clocking 0's out.
				void read(unsigned char* buffer, unsigned int count, bool deselectChip = false);

			protected:
				SPIBus* bus;
				Configuration* configuration;
				DigitalOutput* chipSelect;
		};
	}
}

#endif