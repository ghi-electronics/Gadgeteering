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

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

Load::Load(unsigned char socketNumber)
{
	socket* t_socket = mainboard->getSocket(socketNumber);
	t_socket->ensureTypeIsSupported(socket::types::Y);

    this->P1 = new digital_output(socket, socket::pins::Three, false);
    this->P2 = new digital_output(socket, socket::pins::Four, false);
    this->P3 = new digital_output(socket, socket::pins::Five, false);
    this->P4 = new digital_output(socket, socket::pins::Six, false);
    this->P5 = new digital_output(socket, socket::pins::Seven, false);
    this->P6 = new digital_output(socket, socket::pins::Eight, false);
    this->P7 = new digital_output(socket, socket::pins::Nine, false);
}

Load::~Load()
{
    delete this->P1;
    delete this->P2;
    delete this->P3;
    delete this->P4;
    delete this->P5;
    delete this->P6;
    delete this->P7;
}
