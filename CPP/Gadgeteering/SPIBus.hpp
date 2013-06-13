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
				SPIBus(CPUPin mosi, CPUPin miso, CPUPin sck);
				
				CPUPin mosi;
				CPUPin miso;
				CPUPin sck;

				virtual void writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, SPIDevice::Configuration* configuration);

			public:
				friend class SPIDevice;
				
				virtual ~SPIBus();
		};
	}
}
#endif