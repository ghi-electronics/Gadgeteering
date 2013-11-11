/*
Copyright 2013 GHI Electronics LLC

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

#ifndef _DAISYLINK_COMMAND_BUS_
#define _DAISYLINK_COMMAND_BUS_

#include "Mainboard.h"
#include "I2CBus.h"

#include "Interfaces.h"
#include "Types.h"

namespace GHI
{
	namespace Interfaces
	{
		class DaisyLinkCommandBus : public GHI::Interfaces::I2CBus
		{
			bool startSent;
                                                
			void clearSCL();
			void releaseSCL();
			bool readSCL();
			void clearSDA();
			void releaseSDA();
			bool readSDA();

			void writeBit(bool bit);
			bool readBit();

			void sendStartCondition();
			void sendStopCondition();

			bool transmit(bool sendStart, bool sendStop, unsigned char data);
			unsigned char receive(bool sendAcknowledgeBit, bool sendStopCondition);

			public:
				DaisyLinkCommandBus(GHI::CPUPin sda, GHI::CPUPin scl);
				virtual ~DaisyLinkCommandBus();

				virtual unsigned int write(const unsigned char* buffer, unsigned int count, unsigned char address, bool sendStop = true);
				virtual unsigned int read(unsigned char* buffer, unsigned int count, unsigned char address, bool sendStop = true);
				virtual bool writeRead(const unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength, unsigned int* numWritten, unsigned int* numRead, unsigned char address);
		};
	}
}

#endif