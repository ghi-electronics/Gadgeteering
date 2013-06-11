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
		delete node->data;
		delete prev;
		prev = node;
	} while ((node = node->next) != NULL);

	//Pin reservation is not gauranteed, avoid do:while
	prev = this->pins;
	node = this->pins->next;

	do {
		delete node->data;
		delete prev;
		prev = node;
	} while ((node = node->next) != NULL);
}

void Mainboard::panic(const char* error) {
	while (true)
		; //
}

Socket* Mainboard::registerSocket(Socket* socket) {
	if (this->sockets == NULL) {
		this->sockets = new ListNode();
		this->sockets->next = NULL;
		this->sockets->data = socket;
	
		return socket;
	}
	
	ListNode* endNode;
	for (endNode = this->sockets; endNode->next != NULL; endNode = endNode->next)
		;
	
	endNode->next = new ListNode();
	endNode->next->data = socket;
	endNode->next->next = NULL;

	return socket;
}

Socket* Mainboard::getSocket(int number) {
	if (this->sockets == NULL)
		this->panic("No sockets present.");
	
	for (ListNode* node = this->sockets; node != NULL; node = node->next)
		if (static_cast<Socket*>(node->data)->number == number)
			return static_cast<Socket*>(node->data);

	return NULL;
}

void Mainboard::setPWM(CPUPin pin, double dutyCycle, double frequency) { };
bool Mainboard::readDigital(CPUPin pin) { mainboard->panic("Not Supported"); return false; };
void Mainboard::writeDigital(CPUPin pin, bool value) { };
double Mainboard::readAnalog(CPUPin pin) { mainboard->panic("Not Supported"); return 0.0; };
void Mainboard::writeAnalog(CPUPin pin, double voltage) { };
void Mainboard::setIOMode(CPUPin pin, IOState state, ResistorMode resistorMode) { };
	
GHI::Interfaces::SPIBus* Mainboard::getNewSPIBus(Socket* socket) { mainboard->panic("Not Supported"); return NULL; };
GHI::Interfaces::SerialDevice* Mainboard::getNewSerialDevice(Socket* socket, int baudRate, int parity, int stopBits, int dataBits) { mainboard->panic("Not Supported"); return NULL; };

void Mainboard::ReleasePin(CPUPin pinNumber)
{
	if (this->pins == NULL) {
		this->pins = new ListNode();
		this->pins->next = NULL;
		this->pins->data = NULL;

		return;
	}

	ListNode* node;
	ListNode* prev;

	if (this->pins && this->pins[0].data == (CPUPin*)(pinNumber)) {
		prev = this->pins;
		this->pins = this->pins->next;
		delete prev;
		return;
	}

	prev = this->pins;
	for (node = prev->next; node->next != NULL; node = node->next) {
		if(node->data == (CPUPin*)(pinNumber)) {
			prev->next = node->next;
			delete node;
		}
		prev = node;
	}
}

void Mainboard::ReservePin(CPUPin pinNumber)
{
	return;
	/*
	//Arduino crashes in this code after several calls.
	if (this->pins == NULL) {
		this->pins = new ListNode();
		this->pins->next = NULL;
		this->pins->data = (CPUPin*)pinNumber;
	
		return;
	}
	
	ListNode* node;
	for (node = this->pins; node->next != NULL; node = node->next)
		if (node->data == (CPUPin*)(pinNumber))
			mainboard->panic("Pin already reserved");
	
	node->next = new ListNode();
	node->next->data = (CPUPin*)(pinNumber);
	node->next->next = NULL;
	*/
}
	