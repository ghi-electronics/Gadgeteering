/*
Copyright 2013 Gadgeteering Electronics LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "Joystick.h"

using namespace Gadgeteering;
using namespace Gadgeteering::Modules;
using namespace Gadgeteering::Interfaces;

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
