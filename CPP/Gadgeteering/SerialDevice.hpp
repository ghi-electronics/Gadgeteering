#ifndef _SERIALDEVICE_H_
#define _SERIALDEVICE_H_

#include "Arduino.h"
#include "GHI.h"
#include <SoftwareSerial.h>

class SerialDevice {
	static const Socket::Pin TX = Socket::Pins::Four;
	static const Socket::Pin RX = Socket::Pins::Five;

	Socket* socket;
	SoftwareSerial* port;
	int baudRate;

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

		SerialDevice(Socket* socket, int baudRate, int parity, int stopBits, int dataBits);
		~SerialDevice();
		
		void open();
		void close();
		void write(const unsigned char* buffer, int count);
		void write(const char* buffer, int count);
		void read(byte* buffer, int count);
};

#endif