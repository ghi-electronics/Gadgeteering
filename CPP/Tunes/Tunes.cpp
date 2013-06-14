#include "Tunes.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

Tunes::Tunes(int socketNumber) {
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
