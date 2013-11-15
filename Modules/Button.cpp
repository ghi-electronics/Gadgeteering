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

#include "Button.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

Button::Button(unsigned char socketNumber)
{
	socket* t_socket = mainboard->getSocket(socketNumber);
	t_socket->ensureTypeIsSupported(socket::types::X);

    this->input = new digital_input(socket, socket::pins::Three, resistor_modes::PULL_UP);
	this->led = new digital_output(socket, socket::pins::Four, false);

	this->ledState = false;
}

Button::~Button()
{
	delete this->input;
	delete this->led;
}

bool Button::isPressed()
{
	return !this->input->read();
}

void Button::turnLEDOn()
{
	this->led->write(true);
	this->ledState = true;
}

void Button::turnLEDOff()
{
	this->led->write(false);
	this->ledState = false;
}

void Button::toggleLED()
{
	this->ledState = !this->ledState;
	this->led->write(this->ledState);
}

bool Button::isLEDOn()
{
	return this->ledState;
}