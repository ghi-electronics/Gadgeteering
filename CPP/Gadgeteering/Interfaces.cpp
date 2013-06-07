#include "Interfaces.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

DigitalOutput::DigitalOutput(Socket* socket, Socket::Pin pin, bool initialState) : socket(socket), pin(pin) {
	if (!socket || pin < 3 || pin > 9)
		mainboard->panic("Pin out of range");
	
	mainboard->ReservePin(socket->pins[pin]);

	mainboard->setIOMode(this->socket, this->pin, IOStates::DIGITAL_OUTPUT);
	this->write(initialState);
}

void DigitalOutput::write(bool value) {
	mainboard->writeDigital(this->socket, this->pin, value);
}

DigitalInput::DigitalInput(Socket* socket, Socket::Pin pin) : socket(socket), pin(pin) {
	if (!socket || pin < 3 || pin > 9)
		mainboard->panic("Pin out of range");
	
	mainboard->ReservePin(socket->pins[pin]);

	mainboard->setIOMode(this->socket, this->pin, IOStates::DIGITAL_INPUT, ResistorModes::PULL_UP);
}

bool DigitalInput::read() {
	return mainboard->readDigital(this->socket, this->pin);
}

DigitalInputOutput::DigitalInputOutput(Socket* socket, Socket::Pin pin, IOState initialIOState, bool initialOutputState) : socket(socket), pin(pin) {
	if (!socket || pin < 3 || pin > 9)
		mainboard->panic("Pin out of range");

	this->setState(initialIOState);

	mainboard->ReservePin(socket->pins[pin]);

	if (this->ioState == IOStates::DIGITAL_OUTPUT)
		this->write(initialOutputState);
}

DigitalInputOutput::DigitalInputOutput(Socket::Pin pinNumber, IOState initialIOState, bool initialOutputState) : socket(socket), pin(pinNumber) {

	this->setState(initialIOState);

	mainboard->ReservePin(pinNumber);

	if (this->ioState == IOStates::DIGITAL_OUTPUT)
		this->write(initialOutputState);
}

void DigitalInputOutput::write(bool value) {
	this->setState(IOStates::DIGITAL_OUTPUT);
	mainboard->writeDigital(this->socket, this->pin, value);
}

bool DigitalInputOutput::read() {
	this->setState(IOStates::DIGITAL_INPUT);
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