/*
Copyright 2013 GHI Electronics LLC

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
