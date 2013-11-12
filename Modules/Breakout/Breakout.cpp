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

#include "Breakout.h"

using namespace Gadgeteering;
using namespace Gadgeteering::Modules;
using namespace Gadgeteering::Interfaces;

Breakout::Breakout(unsigned char socketNumber) {
	this->socket = mainboard->getSocket(socketNumber);
}

Breakout::~Breakout() {

}
				
Interfaces::DigitalInput* Breakout::SetupDigitalInput(Socket::Pin pin, ResistorMode resistorMode) {
	return new Interfaces::DigitalInput(this->socket, pin, resistorMode);
}

Interfaces::DigitalOutput* Breakout::SetupDigitalOutput(Socket::Pin pin, bool initialState) {
	return new Interfaces::DigitalOutput(this->socket, pin, initialState);
}

Interfaces::DigitalIO* Breakout::SetupDigitalIO(Socket::Pin pin) {
	return new Interfaces::DigitalIO(this->socket, pin);
}

Interfaces::AnalogInput* Breakout::SetupAnalogInput(Socket::Pin pin) {
	return new Interfaces::AnalogInput(this->socket, pin);
}

Interfaces::AnalogOutput* Breakout::SetupAnalogOutput(Socket::Pin pin) {
	return new Interfaces::AnalogOutput(this->socket, pin);
}

Interfaces::PWMOutput* Breakout::SetupPWMOutput(Socket::Pin pin) {
	return new Interfaces::PWMOutput(this->socket, pin);
}
