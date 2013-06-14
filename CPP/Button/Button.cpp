#include "Button.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

Button::Button(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::X);

    this->input = new DigitalInput(socket, Socket::Pins::Three, ResistorModes::PULL_UP);
	this->led = new DigitalOutput(socket, Socket::Pins::Four, false);

	this->ledState = false;
}

Button::~Button() {
	delete this->input;
	delete this->led;
}

bool Button::isPressed() {
	return !this->input->read();
}

void Button::turnLEDOn() {
	this->led->write(true);
	this->ledState = true;
}

void Button::turnLEDOff() {
	this->led->write(false);
	this->ledState = false;
}

void Button::toggleLED() {
	this->ledState = !this->ledState;
	this->led->write(this->ledState);
}

bool Button::isLEDOn() {
	return this->ledState;
}
