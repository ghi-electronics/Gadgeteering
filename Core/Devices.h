/*
Copyright 2013 Gadgeteering Electronics LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _SPI_H_
#define _SPI_H_

#include "Types.h"
#include "Socket.h"

namespace Gadgeteering {
	namespace Devices {
		class i2c
		{
			unsigned char w_address;
			unsigned char r_address;
			i2c_module module;

			public:
				i2c(Socket& socket, unsigned char address);

				bool write(const unsigned char* buffer, unsigned int count, bool sendStart = true, bool sendStop = true);
				bool read(unsigned char* buffer, unsigned int count, bool sendStart = true, bool sendStop = true);

				bool writeRead(const unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength);

				bool writeRegister(unsigned char address, unsigned char value);
				bool writeRegisters(unsigned char startAddress, unsigned char* values, unsigned int count);
				unsigned char readRegister(unsigned char address);
				bool readRegisters(unsigned char startAddress, unsigned char* values, unsigned int count);
		};

		class spi
		{
			SPIConfiguration config;
			spi_module module;

			public:
				spi(Socket& socket, Socket& chipselect_socket, Socket::Pin chipselect_pin, SPIConfiguration configuration);

				//Clocks in one char and clocks out one char at the same time. If deselectChip is true, the CS line is set to logic low after the transmission, otherwise it remains logic high.
				unsigned char writeReadByte(unsigned char toSend, bool deselectChip = true);

				//Clocks count bytes in and out at the same time to and from the receive and send buffer respectively.
				void writeAndRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, bool deselectChip = true);

				//Clocks sendCount bytes from sendBuffer out while ignoring the received bytes and then clocks receiveCount bytes into the receiveBuffer while clocking 0's out.
				void writeThenRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, bool deselectChip = true);

				//Clocks count bytes out from the buffer while ignoring the bytes clocked in.
				void write(const unsigned char* buffer, unsigned int count, bool deselectChip = true);

				//Clocks count bytes in while clocking 0's out.
				void read(unsigned char* buffer, unsigned int count, bool deselectChip = true);
		};

		class serial
		{
			serial_configuration config;
			serial_module module;

			public:
				serial(Socket& socket, serial_configuration configuration);

				void write(const unsigned char* buffer, unsigned int count);
				void write(const char* buffer, unsigned int count);
				unsigned int read(unsigned char* buffer, unsigned int count);
				unsigned int available();
		};
	}
}

#endif
