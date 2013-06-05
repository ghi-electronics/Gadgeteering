#ifndef _SPI_H_
#define _SPI_H_

#include "Types.hpp"
#include "Socket.hpp"

namespace GHI {
	namespace Interfaces {
		class DigitalOutput;

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

				SPIDevice(Socket* socket, Socket::Pin chipSelectPin, SPIDevice::Configuration* configuration);
				virtual ~SPIDevice();
		
				//Clocks in one byte and clocks out one byte at the same time. If deselectChip is true, the CS line is set to logic low after the transmission, otherwise it remains logic high.
				virtual byte writeReadByte(byte toSend, bool deselectChip = false);

				//Clocks count bytes in and out at the same time to and from the receive and send buffer respectively.
				virtual void writeAndRead(byte* sendBuffer, byte* receiveBuffer, unsigned int count, bool deselectChip = false);

				//Clocks sendCount bytes from sendBuffer out while ignoring the received bytes and then clocks receiveCount bytes into the receiveBuffer while clocking 0's out.
				virtual void writeThenRead(byte* sendBuffer, byte* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, bool deselectChip = false);

				//Clocks count bytes out from the buffer while ignoring the bytes clocked in.
				virtual void write(byte* buffer, unsigned int count, bool deselectChip = false);

				//Clocks count bytes in while clocking 0's out.
				virtual void read(byte* buffer, unsigned int count, bool deselectChip = false);

			protected:
				static const Socket::Pin CLOCK = Socket::Pins::Nine;
				static const Socket::Pin MISO = Socket::Pins::Eight;
				static const Socket::Pin MOSI = Socket::Pins::Seven;

				Socket* socket;
				DigitalOutput* chipSelect;
				Configuration* configuration;
		};
	}
}

#endif