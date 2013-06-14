#include "Tunes.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

Tunes::Tunes(unsigned char socketNumber) {
	this->socket = mainboard->getSocket(socketNumber);
	this->socket->ensureTypeIsSupported(Socket::Types::P);

	this->pwm = new PWMOutput(this->socket, this->PWM_PIN);
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
