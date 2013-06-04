#include "Mainboard.hpp"

Module::Module() {

}

Socket::Socket(int number, Type type) : number(number), type(type) {
	for (int i = 0; i < Socket::PINS_PER_SOCKET; i++)
		this->pins[i] = Socket::PIN_UNCONNECTED;
}

void Socket::ensureTypeIsSupported(Type type) {
	if (this->type & type == 0)
		mainboard->panic("Type not supported");
}

void Socket::setIOMode(Pin pinNumber, IOState state, ResistorMode resistorMode) {
	if (state == IOStates::PWM)
		mainboard->panic("Not supported");

	if (state == IOStates::IN)
		::pinMode(this->pins[pinNumber], resistorMode == ResistorModes::PULL_UP ? INPUT_PULLUP : INPUT);
	else
		::pinMode(this->pins[pinNumber], OUTPUT);
}

bool Socket::readDigital(Pin pinNumber) {
	return ::digitalRead(this->pins[pinNumber]) != 0;
}

void Socket::writeDigital(Pin pinNumber, bool value) {
	::digitalWrite(this->pins[pinNumber], value ? HIGH : LOW);
}

double Socket::readAnalog(Pin pinNumber) {
	return static_cast<double>(::analogRead(this->pins[pinNumber])) / 1024 * 3.3;
}

void Socket::writeAnalog(Pin pinNumber, double voltage) {
	::analogWrite(this->pins[pinNumber], voltage * (1024 / 3.3));
}

void Socket::setPWM(Pin pinNumber, double dutyCycle, double frequency) {
	mainboard->panic("Not supported");
}

Mainboard::Mainboard() {
	this->sockets = NULL;
	
	if (mainboard != NULL)
		mainboard->panic("You can only have one mainboard.");

#ifdef DEBUG_PRINT
	Serial.begin(9600);
#endif
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
