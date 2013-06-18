#include "SPIBus.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

SPIBus::SPIBus(CPUPin mosiPin, CPUPin misoPin, CPUPin sckPin) : mosi(mosiPin), miso(misoPin), sck(sckPin) {	

}

SPIBus::~SPIBus() {

}

void SPIBus::writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration) {
	mainboard->panic(Exceptions::ERR_SPI_NOT_SUPPORTED);
}