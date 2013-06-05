#include "Interfaces.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

DigitalOutput::DigitalOutput(Socket* socket, Socket::Pin pin, bool initialState) : socket(socket), pin(pin) {
	if (!socket || pin < 3 || pin > 9)
		mainboard->panic("Pin out of range");

	mainboard->setIOMode(this->socket, this->pin, IOStates::OUTPUT);
	this->write(initialState);
}

void DigitalOutput::write(bool value) {
	mainboard->writeDigital(this->socket, this->pin, value);
}

DigitalInput::DigitalInput(Socket* socket, Socket::Pin pin) : socket(socket), pin(pin) {
	if (!socket || pin < 3 || pin > 9)
		mainboard->panic("Pin out of range");

	mainboard->setIOMode(this->socket, this->pin, IOStates::INPUT, ResistorModes::PULL_UP);
}

bool DigitalInput::read() {
	return mainboard->readDigital(this->socket, this->pin);
}

DigitalInputOutput::DigitalInputOutput(Socket* socket, Socket::Pin pin, IOState initialIOState, bool initialOutputState) : socket(socket), pin(pin) {
	if (!socket || pin < 3 || pin > 9)
		mainboard->panic("Pin out of range");

	this->setState(initialIOState);

	if (this->ioState == IOStates::OUTPUT)
		this->write(initialOutputState);
}

void DigitalInputOutput::write(bool value) {
	this->setState(IOStates::OUTPUT);
	mainboard->writeDigital(this->socket, this->pin, value);
}

bool DigitalInputOutput::read() {
	this->setState(IOStates::INPUT);
	return mainboard->readDigital(this->socket, this->pin);
}

void DigitalInputOutput::setState(IOState state) {
	if (this->ioState == state)
		return;

	mainboard->setIOMode(this->socket, this->pin, state);
	this->ioState = state;
}

AnalogInput::AnalogInput(Socket* socket, Socket::Pin pin) : socket(socket), pin(pin) {
	if (!socket || pin < 3 || pin > 9)
		mainboard->panic("Pin out of range");
}

double AnalogInput::read() {
	return mainboard->readAnalog(this->socket, this->pin);
}

PWMOutput::PWMOutput(Socket* socket, Socket::Pin pin) : socket(socket), pin(pin) {
	if (!socket || pin < 3 || pin > 9)
		mainboard->panic("Pin out of range");
		
	mainboard->setIOMode(this->socket, this->pin, IOStates::PWM);
	this->set(0, 1000);
}

void PWMOutput::set(double frequency, double dutyCycle) {
	this->dutyCycle = dutyCycle;
	this->frequency = frequency;

	mainboard->setPWM(this->socket, this->pin, this->frequency, this->dutyCycle);
}

void PWMOutput::setFrequency(double frequency) {
	this->set(frequency, this->dutyCycle);
}

void PWMOutput::setDutyCycle(double dutyCycle) {
	this->set(this->frequency, dutyCycle);
}