#include "SPIBus.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

SPIBus::SPIBus(CPUPin mosiPin, CPUPin misoPin, CPUPin sckPin) : mosi(mosiPin), miso(misoPin), sck(sckPin) {	

}

SPIBus::~SPIBus() {
	for (SPIDevice* current = (SPIDevice*)this->spiDevices.start(); !this->spiDevices.ended(); current = (SPIDevice*)this->spiDevices.next())
		delete current;
}

SPIDevice* SPIBus::getSPIDevice(CPUPin pin, SPIConfiguration* configuration) {
	SPIDevice* device = new SPIDevice(this, pin, configuration);
	this->spiDevices.add(device);
	return device; 
}

void SPIBus::writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, GHI::Interfaces::SPIConfiguration* configuration) {
	mainboard->panic(Exceptions::ERR_SPI_NOT_SUPPORTED);
}