#include "Interfaces.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

DigitalOutput::DigitalOutput(Socket* socket, Socket::Pin pin, bool initialState) {
	if (!socket || pin < 3 || pin > 9)
		mainboard->panic("Pin out of range");
	
	this->cpuPin = socket->pins[pin];
	
	mainboard->ReservePin(this->cpuPin);
	mainboard->setIOMode(this->cpuPin, IOStates::DIGITAL_OUTPUT);

	this->write(initialState);
}

void DigitalOutput::write(bool value) {
	mainboard->writeDigital(this->cpuPin, value);
}

DigitalInput::DigitalInput(Socket* socket, Socket::Pin pin, ResistorMode resistorMode) {
	if (!socket || pin < 3 || pin > 9)
		mainboard->panic("Pin out of range");
	
	this->cpuPin = socket->pins[pin];
	
	mainboard->ReservePin(this->cpuPin);
	mainboard->setIOMode(this->cpuPin, IOStates::DIGITAL_INPUT, resistorMode);
}

bool DigitalInput::read() {
	return mainboard->readDigital(this->cpuPin);
}

DigitalInputOutput::DigitalInputOutput(Socket* socket, Socket::Pin pin, IOState initialIOState, bool initialOutputState) {
	if (!socket || pin < 3 || pin > 9)
		mainboard->panic("Pin out of range");

	this->cpuPin = socket->pins[pin];

	this->setState(initialIOState);

	mainboard->ReservePin(this->cpuPin);

	if (this->ioState == IOStates::DIGITAL_OUTPUT)
		this->write(initialOutputState);
}

void DigitalInputOutput::write(bool value) {
	this->setState(IOStates::DIGITAL_OUTPUT);
	mainboard->writeDigital(this->cpuPin, value);
}

bool DigitalInputOutput::read() {
	this->setState(IOStates::DIGITAL_INPUT);
	return mainboard->readDigital(this->cpuPin);
}

void DigitalInputOutput::setState(IOState state) {
	if (this->ioState == state)
		return;

	mainboard->setIOMode(this->cpuPin, state);
	this->ioState = state;
}

AnalogInput::AnalogInput(Socket* socket, Socket::Pin pin) {
	if (!socket || pin < 3 || pin > 9)
		mainboard->panic("Pin out of range");

	this->cpuPin = socket->pins[pin];
}

double AnalogInput::read() {
	return mainboard->readAnalog(this->cpuPin);
}

PWMOutput::PWMOutput(Socket* socket, Socket::Pin pin) {
	if (!socket || pin < 3 || pin > 9)
		mainboard->panic("Pin out of range");

	this->cpuPin = socket->pins[pin];
		
	mainboard->setIOMode(this->cpuPin, IOStates::PWM);
	this->set(0, 1000);
}

void PWMOutput::set(double frequency, double dutyCycle) {
	this->dutyCycle = dutyCycle;
	this->frequency = frequency;

	mainboard->setPWM(this->cpuPin, this->frequency, this->dutyCycle);
}

void PWMOutput::setFrequency(double frequency) {
	this->set(frequency, this->dutyCycle);
}

void PWMOutput::setDutyCycle(double dutyCycle) {
	this->set(this->frequency, dutyCycle);
}