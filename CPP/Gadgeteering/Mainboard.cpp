#include "Mainboard.hpp"

using namespace GHI;

Module::Module() {

}

Mainboard::Mainboard() {
	this->sockets = NULL;
	
	if (mainboard != NULL)
		mainboard->panic("You can only have one mainboard.");
}

Mainboard::~Mainboard() {
	ListNode* prev = this->sockets;
	ListNode* node = this->sockets->next;
	do {
		delete node->socket;
		delete prev;
		prev = node;
	} while ((node = node->next) != NULL);
}

void Mainboard::panic(const char* error) {
	while (true)
		;
}

Socket* Mainboard::registerSocket(Socket* socket) {
	if (this->sockets == NULL) {
		this->sockets = new ListNode();
		this->sockets->next = NULL;
		this->sockets->socket = NULL;
	}

	ListNode* node;
	for (node = this->sockets; node->next != NULL; node = node->next)
		;

	node = node->next = new ListNode();
	node->socket = socket;
	node->next = NULL;

	return socket;
}

Socket* Mainboard::getSocket(int number) {

	if (this->sockets == NULL)
		this->panic("No sockets present.");

	for (ListNode* node = this->sockets; node != NULL; node = node->next)
		if (node->socket->number == number)
			return node->socket;

	return NULL;
}

void Mainboard::setPWM(Socket* socket, Socket::Pin pin, double dutyCycle, double frequency) { };
bool Mainboard::readDigital(Socket* socket, Socket::Pin pin) { };
void Mainboard::writeDigital(Socket* socket, Socket::Pin pin, bool value) { };
double Mainboard::readAnalog(Socket* socket, Socket::Pin pin) { };
void Mainboard::writeAnalog(Socket* socket, Socket::Pin pin, double voltage) { };
void Mainboard::setIOMode(Socket* socket, Socket::Pin pin, IOState state, ResistorMode resistorMode) { };
	
GHI::Interfaces::SPIDevice* Mainboard::getNewSPIDevice(Socket* socket, Socket::Pin chipSelectPin, GHI::Interfaces::SPIDevice::Configuration* configuration) { };
GHI::Interfaces::SerialDevice* Mainboard::getNewSerialDevice(Socket* socket, int baudRate, int parity, int stopBits, int dataBits) { };
	