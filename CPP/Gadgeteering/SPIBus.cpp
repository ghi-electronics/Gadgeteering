#include "SPIBus.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

SPIBus::SPIBus(CPUPin mosiPin, CPUPin misoPin, CPUPin sckPin) : mosi(mosiPin), miso(misoPin), sck(sckPin) {	

}

SPIBus::~SPIBus() {

}

SPIDevice* SPIBus::getSPIDevice(CPUPin pin, SPIConfiguration* configuration) {
	return new SPIDevice(this, pin, configuration);
}
			

void SPIBus::writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, GHI::Interfaces::SPIConfiguration* configuration) {
	mainboard->panic(Exceptions::ERR_SPI_NOT_SUPPORTED);
}