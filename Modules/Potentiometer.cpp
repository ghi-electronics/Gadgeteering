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

#include "Potentiometer.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

Potentiometer::Potentiometer(unsigned char socketNumber)
{
	socket* t_socket = mainboard->getSocket(socketNumber);
	t_socket->ensureTypeIsSupported(socket::types::A);

    this->input = new analog_input(socket, socket::pins::Three);
}

Potentiometer::~Potentiometer()
{
    delete this->input;
}

double Potentiometer::ReadPotentiometerVoltage()
{
    return input->read();
}

double Potentiometer::ReadPotentiometerPercentage()
{
    return (input->read() / 3.3 * 100);
}