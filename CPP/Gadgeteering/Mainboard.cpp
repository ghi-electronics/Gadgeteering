#include "Mainboard.hpp"

using namespace GHI;

Module::Module() {

}

Mainboard::Mainboard() {
	if (mainboard != NULL)
		mainboard->panic("You can only have one mainboard.");
}

Mainboard::~Mainboard() {

}

void Mainboard::panic(const char* error) {
	while (true)
		;
}

Socket* Mainboard::registerSocket(Socket* socket) {
	this->sockets.add(socket);

	return socket;
}

Socket* Mainboard::getSocket(int number) {
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
		mainboard->panic("Pin already reserved.");
	this->pins.add((void*)pin);
}

void Mainboard::setPWM(CPUPin pin, double dutyCycle, double frequency) { mainboard->panic("SetPWM not supported."); };
bool Mainboard::readDigital(CPUPin pin) { mainboard->panic("ReadDigital not supported."); return false; };
void Mainboard::writeDigital(CPUPin pin, bool value) { mainboard->panic("WriteDigital not supported."); };
double Mainboard::readAnalog(CPUPin pin) { mainboard->panic("ReadAnalog not supported."); return 0.0; };
void Mainboard::writeAnalog(CPUPin pin, double voltage) { mainboard->panic("WriteAnalog not supported."); };
void Mainboard::setIOMode(CPUPin pin, IOState state, ResistorMode resistorMode) { mainboard->panic("SetIOMode not supported."); };
	
GHI::Interfaces::SPIBus* Mainboard::getNewSPIBus(CPUPin mosiPin, CPUPin misoPin, CPUPin sckPin) { mainboard->panic("GetNewSPIBus not supported."); return NULL; };
GHI::Interfaces::SPIBus* Mainboard::getNewSPIBus(Socket* socket, Socket::Pin mosiPinNumber, Socket::Pin misoPinNumber, Socket::Pin sckPinNumber) { mainboard->panic("GetNewSPIBus not supported."); return NULL; };
GHI::Interfaces::SerialDevice* Mainboard::getNewSerialDevice(CPUPin txPin, CPUPin rxPin, int baudRate, int parity, int stopBits, int dataBits) { mainboard->panic("GetNewSerialDevice not supported."); return NULL; };
GHI::Interfaces::SerialDevice* Mainboard::getNewSerialDevice(Socket* socket, Socket::Pin txPinNumber, Socket::Pin rxPinNumber, int baudRate, int parity, int stopBits, int dataBits) { mainboard->panic("GetNewSerialDevice not supported."); return NULL; };
	