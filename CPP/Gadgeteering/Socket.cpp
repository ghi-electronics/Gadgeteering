#include "Socket.hpp"
#include "Mainboard.hpp"
#include "I2CBus.hpp"
#include "SPIBus.hpp"
#include "SerialDevice.hpp"

using namespace GHI;

Socket::Socket(unsigned char number, Type type) : number(number), type(type) {
	for (unsigned char i = 0; i < 10; i++)
		this->pins[i] = Socket::Pins::Unconnected;
}

void Socket::ensureTypeIsSupported(Type type) {
	if ((this->type & type) == 0)
		mainboard->panic(Exceptions::ERR_SOCKET_NOT_HAVE_TYPE);
}

Interfaces::I2CBus* Socket::getI2CBus(Pin sda, Pin scl) {
	return mainboard->getI2CBus(this, sda, scl);
}

Interfaces::SerialDevice* Socket::getSerialDevice(unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits, Pin tx, Pin rx){
	return mainboard->getSerialDevice(baudRate, parity, stopBits, dataBits, this, tx, rx);

}

Interfaces::SPIBus* Socket::getSPIBus(Pin mosi, Pin miso, Pin sck){
	return mainboard->getSPIBus(this, mosi, miso, sck);
}
