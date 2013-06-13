#include "SPIBus.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

SPIBus::SPIBus(CPUPin mosi, CPUPin miso, CPUPin sck) {	
	this->mosi = mosi;
	this->miso = miso;
	this->sck = sck;
}

SPIBus::~SPIBus() {

}

void SPIBus::writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration) {
	mainboard->panic(ERR_SPI_NOT_SUPPORTED);
}