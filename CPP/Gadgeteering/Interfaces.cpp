#include "Interfaces.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

DigitalOutput::DigitalOutput(Socket* socket, Socket::Pin pinNumber, bool initialState) {
	if (!socket)
		mainboard->panic(Exceptions::ERR_INVALID_SOCKET);
	if (pinNumber < 3 || pinNumber > 9)
		mainboard->panic(Exceptions::ERR_PIN_OUT_OF_RANGE);
	
	this->cpuPin = socket->pins[pinNumber];
	
	mainboard->ReservePin(this->cpuPin);
	mainboard->setIOMode(this->cpuPin, IOStates::DIGITAL_OUTPUT);

	this->write(initialState);
}

DigitalOutput::DigitalOutput(CPUPin pin, bool initialState) {
	this->cpuPin = pin;
	
	mainboard->ReservePin(this->cpuPin);
	mainboard->setIOMode(this->cpuPin, IOStates::DIGITAL_OUTPUT);

	this->write(initialState);
}

void DigitalOutput::write(bool value) {
	mainboard->writeDigital(this->cpuPin, value);
}

DigitalInput::DigitalInput(Socket* socket, Socket::Pin pinNumber, ResistorMode resistorMode) {
	if (!socket)
		mainboard->panic(Exceptions::ERR_INVALID_SOCKET);
	if (pinNumber < 3 || pinNumber > 9)
		mainboard->panic(Exceptions::ERR_PIN_OUT_OF_RANGE);
	
	this->cpuPin = socket->pins[pinNumber];
	
	mainboard->ReservePin(this->cpuPin);
	mainboard->setIOMode(this->cpuPin, IOStates::DIGITAL_INPUT, resistorMode);
}

bool DigitalInput::read() {
	return mainboard->readDigital(this->cpuPin);
}

void DigitalInput::setResistorMode(ResistorMode resistorMode) {
	mainboard->setIOMode(this->cpuPin, IOStates::DIGITAL_INPUT, resistorMode);
}

DigitalInputOutput::DigitalInputOutput(Socket* socket, Socket::Pin pinNumber) {
	if (!socket)
		mainboard->panic(Exceptions::ERR_INVALID_SOCKET);
	if (pinNumber < 3 || pinNumber > 9)
		mainboard->panic(Exceptions::ERR_PIN_OUT_OF_RANGE);

	this->cpuPin = socket->pins[pinNumber];
	mainboard->ReservePin(this->cpuPin);

	this->resistorMode == ResistorModes::FLOATING;
	this->ioState = static_cast<IOState>(0xFF);
}

DigitalInputOutput::DigitalInputOutput(CPUPin pin) {
	this->cpuPin = pin;
	mainboard->ReservePin(this->cpuPin);

	this->resistorMode == ResistorModes::FLOATING;
	this->ioState = static_cast<IOState>(0xFF);
}

void DigitalInputOutput::write(bool value) {
	this->setIOState(IOStates::DIGITAL_OUTPUT);
	mainboard->writeDigital(this->cpuPin, value);
}

bool DigitalInputOutput::read() {
	this->setIOState(IOStates::DIGITAL_INPUT);
	return mainboard->readDigital(this->cpuPin);
}

void DigitalInputOutput::setIOState(IOState state) {
	if (this->ioState == state)
		return;
	
	this->ioState = state;
	mainboard->setIOMode(this->cpuPin, this->ioState, this->resistorMode);
}

void DigitalInputOutput::setResistorMode(ResistorMode mode) {
	if (this->resistorMode == mode)
		return;
	
	this->resistorMode = mode;
	mainboard->setIOMode(this->cpuPin, this->ioState, this->resistorMode);
}

AnalogInput::AnalogInput(Socket* socket, Socket::Pin pinNumber) {
	if (!socket)
		mainboard->panic(Exceptions::ERR_INVALID_SOCKET);
	if (!socket || pinNumber < 3 || pinNumber > 9)
		mainboard->panic(Exceptions::ERR_PIN_OUT_OF_RANGE);

	this->cpuPin = socket->pins[pinNumber];
	mainboard->ReservePin(this->cpuPin);
}

AnalogInput::AnalogInput(CPUPin pin) {
	this->cpuPin = pin;
	mainboard->ReservePin(this->cpuPin);
}

double AnalogInput::read() {
	return mainboard->readAnalog(this->cpuPin);
}

PWMOutput::PWMOutput(Socket* socket, Socket::Pin pinNumber) {
	if (!socket)
		mainboard->panic(Exceptions::ERR_INVALID_SOCKET);
	if (pinNumber < 3 || pinNumber > 9)
		mainboard->panic(Exceptions::ERR_PIN_OUT_OF_RANGE);

	this->cpuPin = socket->pins[pinNumber];
		
	mainboard->setIOMode(this->cpuPin, IOStates::PWM);
	mainboard->ReservePin(this->cpuPin);

	this->set(0, 0);
}

PWMOutput::PWMOutput(CPUPin pin) {
	this->cpuPin = pin;
		
	mainboard->setIOMode(this->cpuPin, IOStates::PWM);
	mainboard->ReservePin(this->cpuPin);

	this->set(0, 0);
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