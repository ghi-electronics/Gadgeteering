#include "Joystick.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

Joystick::Joystick(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::A);

    this->button = new DigitalInput(socket, Socket::Pins::Three, ResistorModes::PULL_UP);
	this->x = new AnalogInput(socket, Socket::Pins::Four);
	this->y = new AnalogInput(socket, Socket::Pins::Five);
}

Joystick::~Joystick() {
	delete this->button;
	delete this->x;
	delete this->y;
}

bool Joystick::isPressed() {
	return !this->button->read();
}

void Joystick::getXY(double* x, double* y) {
	if (x) *x = this->getX();
	if (y) *y = this->getY();
}

double Joystick::getX() {
	return this->x->readProportion();
}

double Joystick::getY() {
	return 1.0 - this->y->readProportion();
}
