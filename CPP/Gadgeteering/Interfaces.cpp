#include "Interfaces.hpp"

using namespace GHI::Interfaces;

DigitalOutput::DigitalOutput(Socket* socket, Socket::Pin pin, bool initialState) : socket(socket), pin(pin) {
	if (!socket || pin < 3 || pin > 9)
		mainboard->panic("Pin out of range");

	this->socket->setIOMode(this->pin, Socket::IOStates::OUT);
	this->write(initialState);
}

void DigitalOutput::write(bool value) {
	this->socket->writeDigital(this->pin, value);
}

DigitalInput::DigitalInput(Socket* socket, Socket::Pin pin) : socket(socket), pin(pin) {
	if (!socket || pin < 3 || pin > 9)
		mainboard->panic("Pin out of range");

	this->socket->setIOMode(this->pin, Socket::IOStates::IN, Socket::ResistorModes::PULL_UP);
}

bool DigitalInput::read() {
	return this->socket->readDigital(this->pin);
}

DigitalInputOutput::DigitalInputOutput(Socket* socket, Socket::Pin pin, Socket::IOState initialIOState, bool initialOutputState) : socket(socket), pin(pin) {
	if (!socket || pin < 3 || pin > 9)
		mainboard->panic("Pin out of range");

	this->setState(initialIOState);

	if (this->ioState == Socket::IOStates::OUT)
		this->write(initialOutputState);
}

void DigitalInputOutput::write(bool value) {
	this->setState(Socket::IOStates::OUT);
	this->socket->writeDigital(this->pin, value);
}

bool DigitalInputOutput::read() {
	this->setState(Socket::IOStates::IN);
	return this->socket->readDigital(this->pin);
}

void DigitalInputOutput::setState(Socket::IOState state) {
	if (this->ioState == state)
		return;

	this->socket->setIOMode(this->pin, state);
	this->ioState = state;
}

AnalogInput::AnalogInput(Socket* socket, Socket::Pin pin) : socket(socket), pin(pin) {
	if (!socket || pin < 3 || pin > 9)
		mainboard->panic("Pin out of range");
}

double AnalogInput::read() {
	return this->socket->readAnalog(this->pin);
}

PWMOutput::PWMOutput(Socket* socket, Socket::Pin pin) : socket(socket), pin(pin) {
	if (!socket || pin < 3 || pin > 9)
		mainboard->panic("Pin out of range");
		
	this->socket->setIOMode(this->pin, Socket::IOStates::PWM);
	this->set(0, 1000);
}

void PWMOutput::set(double frequency, double dutyCycle) {
	this->dutyCycle = dutyCycle;
	this->frequency = frequency;

	this->socket->setPWM(this->pin, this->frequency, this->dutyCycle);
}

void PWMOutput::setFrequency(double frequency) {
	this->set(frequency, this->dutyCycle);
}

void PWMOutput::setDutyCycle(double dutyCycle) {
	this->set(this->frequency, dutyCycle);
}