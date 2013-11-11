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

#include "KeypadKP16.h"

using namespace Gadgeteering;
using namespace Gadgeteering::Modules;
using namespace Gadgeteering::Interfaces;

KeypadKP16::KeypadKP16(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::Y);

	this->A0 = new DigitalOutput(socket, Socket::Pins::Three, true);
    this->A1 = new DigitalOutput(socket, Socket::Pins::Four, true);
    this->R1 = new DigitalInput(socket, Socket::Pins::Five, ResistorModes::PULL_UP);
    this->R2 = new DigitalInput(socket, Socket::Pins::Six, ResistorModes::PULL_UP);
    this->R3 = new DigitalInput(socket, Socket::Pins::Seven, ResistorModes::PULL_UP);
    this->R4 = new DigitalInput(socket, Socket::Pins::Eight, ResistorModes::PULL_UP);
}

KeypadKP16::~KeypadKP16() {
	delete this->A0;
    delete this->A1;
    delete this->R1;
    delete this->R2;
    delete this->R3;
    delete this->R4;
}

bool KeypadKP16::IsKeyPressed(Key key)
{
	bool a0 = false;
	bool a1 = false;
	
	if (key == Keys::One || key == Keys::Four || key == Keys::Seven || key == Keys::Star) { a0 = false; a1 = false; }
	else if (key == Keys::Two || key == Keys::Five || key == Keys::Eight || key == Keys::Zero) { a0 = true; a1 = false; }
	else if (key == Keys::Three || key == Keys::Six || key == Keys::Nine || key == Keys::Pound) { a0 = false; a1 = true; }
	else if (key == Keys::A || key == Keys::B || key == Keys::C || key == Keys::D) { a0 = true; a1 = true; }
	
	this->A0->write(a0);
	this->A1->write(a1);
	
	if (key == Keys::One || key == Keys::Two || key == Keys::Three || key == Keys::A) return !this->R1->read();
	else if (key == Keys::Four || key == Keys::Five || key == Keys::Six || key == Keys::B) return !this->R2->read();
	else if (key == Keys::Seven || key == Keys::Eight || key == Keys::Nine || key == Keys::C) return !this->R3->read();
	else if (key == Keys::Star || key == Keys::Zero || key == Keys::Pound || key == Keys::D) return !this->R4->read();

	return false;
}