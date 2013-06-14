#include "Mainboard.hpp"

using namespace GHI;

Module::Module() {

}

Mainboard::Mainboard() {
	if (mainboard != NULL)
		mainboard->panic(ERR_ONLY_ONE_MAINBOARD);
}

Mainboard::~Mainboard() {

}

void Mainboard::panic(unsigned char error) {
	while (true)
		;
}

Socket* Mainboard::registerSocket(Socket* socket) {
	this->sockets.add(socket);

	return socket;
}

Socket* Mainboard::getSocket(unsigned char number) {
	for (Socket* current = (Socket*)this->sockets.start(); !this->sockets.ended(); current = (Socket*)this->sockets.next())
		if (current->number == number)
			return current;

	return NULL;
}

void Mainboard::ReleasePin(CPUPin pin)
{
	this->pins.remove((void*)pin);
}

void Mainboard::ReservePin(CPUPin pin)
{
	if (this->pins.contains((void*)pin))
		mainboard->panic(ERR_PIN_RESERVED);
	this->pins.add((void*)pin);
}

void Mainboard::setPWM(CPUPin pin, double dutyCycle, double frequency) { mainboard->panic(ERR_PWM_NOT_SUPPORTED); };
bool Mainboard::readDigital(CPUPin pin) { mainboard->panic(ERR_READ_DIGITAL_NOT_SUPPORTED); return false; };
void Mainboard::writeDigital(CPUPin pin, bool value) { mainboard->panic(ERR_WRITE_DIGITAL_NOT_SUPPORTED); };
double Mainboard::readAnalog(CPUPin pin) { mainboard->panic(ERR_READ_ANALOG_NOT_SUPPORTED); return 0.0; };
void Mainboard::writeAnalog(CPUPin pin, double voltage) { mainboard->panic(ERR_WRITE_ANALOG_NOT_SUPPORTED); };
void Mainboard::setIOMode(CPUPin pin, IOState state, ResistorMode resistorMode) { mainboard->panic(ERR_SET_IO_NOT_SUPPORTED); };
	
GHI::Interfaces::SPIBus* Mainboard::getNewSPIBus(CPUPin mosiPin, CPUPin misoPin, CPUPin sckPin) { mainboard->panic(ERR_SPI_NOT_SUPPORTED); return NULL; };
GHI::Interfaces::SPIBus* Mainboard::getNewSPIBus(Socket* socket, Socket::Pin mosiPinNumber, Socket::Pin misoPinNumber, Socket::Pin sckPinNumber) { mainboard->panic(ERR_SPI_NOT_SUPPORTED); return NULL; };
//GHI::Interfaces::SerialDevice* Mainboard::getNewSerialDevice(CPUPin txPin, CPUPin rxPin, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits) { mainboard->panic(ERR_SERIAL_NOT_SUPPORTED); return NULL; };
//GHI::Interfaces::SerialDevice* Mainboard::getNewSerialDevice(Socket* socket, Socket::Pin txPinNumber, Socket::Pin rxPinNumber, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits) { mainboard->panic(ERR_SERIAL_NOT_SUPPORTED); return NULL; };
