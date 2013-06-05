#ifndef _SERIALDEVICE_H_
#define _SERIALDEVICE_H_

#include "Types.hpp"
#include "Socket.hpp"

namespace GHI {
	namespace Interfaces {
		class SerialDevice {
			protected:
				static const Socket::Pin TX = Socket::Pins::Four;
				static const Socket::Pin RX = Socket::Pins::Five;

				Socket* socket;
				int baudRate;
				int parity;
				int stopBits;
				int dataBits;

				SerialDevice(Socket* socket, int baudRate, int parity, int stopBits, int dataBits);

			public:
				class Parity {
					public:
						static const int EVEN = 0;
						static const int ODD = 1;
						static const int MARK = 1;
						static const int SPACE = 1;
						static const int NONE = 1;
				};

				class StopBits {
					public:
						static const int NONE = 0;
						static const int ONE = 0;
						static const int ONE_POINT_FIVE = 0;
						static const int TWO = 0;
				};

				virtual ~SerialDevice();
		
				virtual void open();
				virtual void close();
				virtual void write(const unsigned char* buffer, int count);
				virtual void write(const char* buffer, int count);
				virtual void read(char* buffer, int count);
		};
	}
}

#endif