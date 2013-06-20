#ifndef _SPIBUS_H_
#define _SPIBUS_H_

#include "Socket.hpp"
#include "SPIDevice.hpp"
#include "Interfaces.hpp"
#include "Types.hpp"

namespace GHI
{
	namespace Interfaces
	{
		class SPIBus
		{
			public:
				const CPUPin mosi;
				const CPUPin miso;
				const CPUPin sck;

				SPIBus(CPUPin mosiPin, CPUPin misoPin, CPUPin sckPin);
				virtual ~SPIBus();

				SPIDevice* getSPIDevice(CPUPin pin, SPIConfiguration* configuration);

				virtual void writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, SPIConfiguration* configuration);
		};
	}
}
#endif