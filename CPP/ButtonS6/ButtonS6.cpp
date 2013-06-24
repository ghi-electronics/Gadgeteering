#include "ButtonS6.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

ButtonS6::ButtonS6(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::Y);
	
	unsigned char buttonMap[6] = {3, 4, 6, 7, 8, 9};

	for (unsigned char i = 0; i < 6; i++)
		this->buttons[i] = new DigitalInput(socket, buttonMap[i], ResistorModes::PULL_UP);

    this->led = new DigitalOutput(socket, Socket::Pins::Five, false);

	this->ledState = false;
}

ButtonS6::~ButtonS6() {
	for (unsigned char i = 0; i < 6; i++)
		delete this->buttons[i];

    delete this->led;
}

bool ButtonS6::isPressed(Button buttonNumber) {
	if (buttonNumber > 6 || buttonNumber < 1)
		mainboard->panic(Exceptions::ERR_MODULE_ERROR, 1);

	return !this->buttons[buttonNumber - 1]->read();
}

void ButtonS6::turnLEDOn() {
	this->led->write(true);
	this->ledState = true;
}

void ButtonS6::turnLEDOff() {
	this->led->write(false);
	this->ledState = false;
}

void ButtonS6::toggleLED() {
	this->ledState = !this->ledState;
	this->led->write(this->ledState);
}

bool ButtonS6::isLEDOn() {
	return this->ledState;
}
