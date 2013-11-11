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

#ifndef _SERIALDEVICE_H_
#define _SERIALDEVICE_H_

#include "Types.h"
#include "Socket.h"

namespace Gadgeteering {
	namespace Interfaces {
		class SerialDevice {
			protected:
				unsigned int baudRate;
				unsigned char parity;
				unsigned char stopBits;
				unsigned char dataBits;

				SerialDevice(CPUPin txPin, CPUPin rxPin, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits);

			public:
				const CPUPin tx;
				const CPUPin rx;

				class Parity {
					public:
						static const unsigned char EVEN = 0;
						static const unsigned char ODD = 1;
						static const unsigned char MARK = 2;
						static const unsigned char SPACE = 3;
						static const unsigned char NONE = 4;
				};

				class StopBits {
					public:
						static const unsigned char NONE = 0;
						static const unsigned char ONE = 1;
						static const unsigned char ONE_POINT_FIVE = 2;
						static const unsigned char TWO = 3;
				};

				virtual ~SerialDevice();
		
				virtual void open();
				virtual void close();
				virtual void write(const unsigned char* buffer, unsigned int count);
				void write(const char* buffer, unsigned int count);
				virtual unsigned int read(unsigned char* buffer, unsigned int count);
				virtual unsigned int available();
		};
	}
}

#endif