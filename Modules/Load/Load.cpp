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

#include "Load.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

Load::Load(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::Y);
	
    this->P1 = new DigitalOutput(socket, Socket::Pins::Three, false);
    this->P2 = new DigitalOutput(socket, Socket::Pins::Four, false);
    this->P3 = new DigitalOutput(socket, Socket::Pins::Five, false);
    this->P4 = new DigitalOutput(socket, Socket::Pins::Six, false);
    this->P5 = new DigitalOutput(socket, Socket::Pins::Seven, false);
    this->P6 = new DigitalOutput(socket, Socket::Pins::Eight, false);
    this->P7 = new DigitalOutput(socket, Socket::Pins::Nine, false);
}

Load::~Load() {
    delete this->P1;
    delete this->P2;
    delete this->P3;
    delete this->P4;
    delete this->P5;
    delete this->P6;
    delete this->P7;
}