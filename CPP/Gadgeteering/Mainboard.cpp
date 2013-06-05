#include "Mainboard.hpp"

using namespace GHI;

Module::Module() {

}

Mainboard::Mainboard() {
	this->sockets = NULL;
	
	if (mainboard != NULL)
		mainboard->panic("You can only have one mainboard.");

#ifdef DEBUG_PRINT
	Serial.begin(9600);
#endif
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
	#ifdef DEBUG_PRINT
		Serial.println(error);
	#else
		;
	#endif
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
