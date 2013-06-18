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
			protected:
				SPIBus(CPUPin mosiPin, CPUPin misoPin, CPUPin sckPin);

				virtual void writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, SPIDevice::Configuration* configuration);

			public:
				friend class SPIDevice;

				const CPUPin mosi;
				const CPUPin miso;
				const CPUPin sck;
				
				virtual ~SPIBus();
		};
	}
}
#endif