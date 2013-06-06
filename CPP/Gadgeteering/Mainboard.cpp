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
		delete node->node;
		delete prev;
		prev = node;
	} while ((node = node->next) != NULL);

	//Pin reservation is not garunteed, avoid do:while
	prev = this->pins;
	node = this->pins->next;

	do {
		delete node->node;
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
		this->sockets->node = NULL;
	}

	ListNode* node;
	for (node = this->sockets; node->next != NULL; node = node->next)
		;

	node = node->next = new ListNode();
	node->node = socket;
	node->next = NULL;

	return socket;
}

Socket* Mainboard::getSocket(int number) {

	if (this->sockets == NULL)
		this->panic("No sockets present.");

	for (ListNode* node = this->sockets; node != NULL; node = node->next)
		if (static_cast<Socket*>(node->node)->number == number)
			return static_cast<Socket*>(node->node);

	return NULL;
}

void Mainboard::setPWM(Socket* socket, Socket::Pin pin, double dutyCycle, double frequency) { };
bool Mainboard::readDigital(Socket* socket, Socket::Pin pin) { mainboard->panic("Not Supported"); return false; };
void Mainboard::writeDigital(Socket* socket, Socket::Pin pin, bool value) { };
double Mainboard::readAnalog(Socket* socket, Socket::Pin pin) { mainboard->panic("Not Supported"); return 0.0; };
void Mainboard::writeAnalog(Socket* socket, Socket::Pin pin, double voltage) { };
void Mainboard::setIOMode(Socket* socket, Socket::Pin pin, IOState state, ResistorMode resistorMode) { };
	
GHI::Interfaces::SPIBus* Mainboard::getNewSPIBus(Socket* socket) { mainboard->panic("Not Supported"); return NULL; };
GHI::Interfaces::SerialDevice* Mainboard::getNewSerialDevice(Socket* socket, int baudRate, int parity, int stopBits, int dataBits) { mainboard->panic("Not Supported"); return NULL; };

void Mainboard::ReleasePin(Socket::Pin pinNumber)
{
	if (this->pins == NULL) {
		this->pins = new ListNode();
		this->pins->next = NULL;
		this->pins->node = NULL;

		return;
	}

	ListNode* node;
	for (node = this->pins; node->next != NULL; node = node->next)
	{
		if(node->node == (int *)(pinNumber))
			node->node = node->next;
	}
}

void Mainboard::ReservePin(Socket::Pin pinNumber)
{
	if (this->pins == NULL) {
		this->pins = new ListNode();
		this->pins->next = NULL;
		this->pins->node = NULL;
	}

	ListNode* node;
	for (node = this->pins; node->next != NULL; node = node->next)
	{
		if(node->node == (int *)(pinNumber))
			mainboard->panic("Pin already reserved");
	}

	node = node->next = new ListNode();
	node->node = (int *)(pinNumber);
	node->next = NULL;
}
	