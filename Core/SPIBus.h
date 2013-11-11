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

#ifndef _SPIBUS_H_
#define _SPIBUS_H_

#include "Socket.h"
#include "SPIDevice.h"
#include "Interfaces.h"
#include "Types.h"
#include "List.h"

namespace Gadgeteering
{
	namespace Interfaces
	{
		class SPIBus
		{
			List spiDevices;

			public:
				const CPUPin mosi;
				const CPUPin miso;
				const CPUPin sck;

				SPIBus(CPUPin mosiPin, CPUPin misoPin, CPUPin sckPin);
				virtual ~SPIBus();

				SPIDevice* getSPIDevice(CPUPin chipSelectPin, SPIConfiguration* configuration);

				virtual void writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, SPIConfiguration* configuration, bool deselectAfter);
		};
	}
}
#endif