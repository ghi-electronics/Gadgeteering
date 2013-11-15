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

#pragma once

#include "Mainboard.h"
#include "Interfaces.h"
#include "Types.h"
#include "Devices.h"

namespace gadgeteering
{
	class DaisyLinkModule;

	class DaisyLinkBus {
		private:
			static const socket_pin_number DAISYLINK_PIN = 3;
			static const socket_pin_number SDA_PIN = 4;
			static const socket_pin_number SCL_PIN = 5;
			static const unsigned char DEFAULT_I2C_ADDRESS = 0x7F;

			struct link_node
			{
				link_node* next;
				DaisyLinkBus* data;
			};
			static link_node* daisyLinkList;

			interfaces::digital_io* daisyLinkResetPort;

			static unsigned char totalNodeCount;
			
			bool ready;
			unsigned char nodeCount;
			unsigned char reservedCount;
			unsigned char startAddress;
			const socket& sock;
			devices::i2c* i2c;

			DaisyLinkBus(const socket& sock, DaisyLinkModule* module);
			~DaisyLinkBus();

			unsigned char ReserveNextDaisyLinkNodeAddress(DaisyLinkModule* moduleInstance);
			void Initialize();
			void SendResetPulse();
			void GetModuleParameters(unsigned int position, unsigned char* manufacturer, unsigned char* type, unsigned char* version);
			unsigned char GetDaisyLinkVersion(unsigned int position);
			unsigned char ReadRegister(unsigned char registerAddress, unsigned char moduleAddress = DaisyLinkBus::DEFAULT_I2C_ADDRESS);

		public:
			typedef unsigned char Register;

			class Registers {
				public:
					static const Register ADDRESS = 0;
					static const Register CONFIG = 1;
					static const Register DAISYLINK_VERSION = 2;
					static const Register MODULE_TYPE = 3;
					static const Register MODULE_VERSION = 4;
					static const Register MANUFACTURER = 5;
			};
			
			bool IsReady() const;
			unsigned char GetNodeCount() const;
			unsigned char GetReservedCount() const;
			unsigned char GetStartAddress() const;
			const socket& GetSocket() const;

			static DaisyLinkBus* GetDaisyLinkForSocket(const socket& sock, DaisyLinkModule* module);

			friend class DaisyLinkModule;
	};

	class DaisyLinkModule {
		protected:
			DaisyLinkBus* daisyLink;

			static const unsigned char OFFSET = 8;
			static const unsigned char VERSION_IMPLEMENTED = 4;

			unsigned char ModuleAddress;
			unsigned char Manufacturer;
			unsigned char ModuleType;
			unsigned char ModuleVersion;
			unsigned char DaisyLinkVersion;

			unsigned int PositionOnChain;
			unsigned int LengthOfChain;
			unsigned int DaisyLinkSocketNumber;

			DaisyLinkModule(unsigned char socketNumber, unsigned char manufacturer, unsigned char moduleType, unsigned char minModuleVersionSupported, unsigned char maxModuleVersionSupported);
			virtual ~DaisyLinkModule();

			static unsigned int GetLengthOfChain(unsigned char socketNumber);
			static void GetModuleParameters(unsigned char socketNumber, unsigned int position, unsigned char* manufacturer, unsigned char* type, unsigned char* version);

			unsigned char Read(unsigned char memoryAddress);
			void Write(const unsigned char* buffer, unsigned int length);
			void WriteRead(const unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength, unsigned int* numWritten, unsigned int* numRead);

		public:
			unsigned int GetPositionOnChain() const;
			unsigned int GetLengthOfChain() const;
			unsigned int GetDaisyLinkSocketNumber() const;

			friend class DaisyLinkBus;
	};
}
