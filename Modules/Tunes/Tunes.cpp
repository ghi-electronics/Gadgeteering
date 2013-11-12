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

#include "Tunes.h"

using namespace Gadgeteering;
using namespace Gadgeteering::Modules;
using namespace Gadgeteering::Interfaces;

Tunes::Tunes(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::P);

	this->pwm = new PWMOutput(socket, this->PWM_PIN);
}

Tunes::~Tunes() {
	delete this->pwm;
}

void Tunes::set(double frequency, double dutyCycle) {
	this->pwm->set(frequency, dutyCycle);
}

void Tunes::setFrequency(double frequency) {
	this->pwm->setFrequency(frequency);
}

void Tunes::setDutyCycle(double dutyCycle) {
	this->pwm->setDutyCycle(dutyCycle);
}
