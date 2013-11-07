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

#include "ButtonS7.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

ButtonS7::ButtonS7(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::Y);
	
	for (unsigned char i = 0; i < 7; i++)
		this->buttons[i] = new DigitalInput(socket, i + 3, ResistorModes::PULL_UP);
}

ButtonS7::~ButtonS7() {
	for (unsigned char i = 0; i < 6; i++)
		delete this->buttons[i];
}

bool ButtonS7::isPressed(Button buttonNumber) {
	if (buttonNumber > 9 || buttonNumber < 3)
		mainboard->panic(Exceptions::ERR_MODULE_ERROR, 1);

	return !this->buttons[buttonNumber - 3]->read();
}