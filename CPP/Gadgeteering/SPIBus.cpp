#include "SPIBus.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

SPIBus::SPIBus(CPUPin mosi, CPUPin miso, CPUPin sck) : mosi(mosi), miso(miso), sck(sck) {	

}

SPIBus::~SPIBus() {

}

void SPIBus::writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration) {
	mainboard->panic(Exceptions::ERR_SPI_NOT_SUPPORTED);
}