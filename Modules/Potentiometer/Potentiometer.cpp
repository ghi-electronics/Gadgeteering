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

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

Potentiometer::Potentiometer(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::A);
	
    this->input = new AnalogInput(socket, Socket::Pins::Three);
}

Potentiometer::~Potentiometer() {
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
