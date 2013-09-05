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

#include "Interfaces.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

DigitalOutput::DigitalOutput(Socket* socket, Socket::Pin pinNumber, bool initialState) {
	if (!socket)
		mainboard->panic(Exceptions::ERR_INVALID_SOCKET);
	if (pinNumber < 3 || pinNumber > 9)
		mainboard->panic(Exceptions::ERR_PIN_OUT_OF_RANGE);
	
	this->cpuPin = socket->pins[pinNumber];
	
	mainboard->setIOMode(this->cpuPin, IOStates::DIGITAL_OUTPUT);

	this->write(initialState);
}

DigitalOutput::DigitalOutput(CPUPin pin, bool initialState) {
	this->cpuPin = pin;
	
	mainboard->setIOMode(this->cpuPin, IOStates::DIGITAL_OUTPUT);

	this->write(initialState);
}

DigitalOutput::~DigitalOutput() {

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
	this->setResistorMode(resistorMode);
}

DigitalInput::DigitalInput(CPUPin pin, ResistorMode resistorMode)
{
	this->cpuPin = pin;
	this->setResistorMode(resistorMode);
}

DigitalInput::~DigitalInput() {

}

bool DigitalInput::read() {
	return mainboard->readDigital(this->cpuPin);
}

void DigitalInput::setResistorMode(ResistorMode resistorMode) {
	this->resistorMode = resistorMode;
	mainboard->setIOMode(this->cpuPin, IOStates::DIGITAL_INPUT, resistorMode);
}

ResistorMode DigitalInput::getResistorMode() {
	return this->resistorMode;
}

DigitalIO::DigitalIO(Socket* socket, Socket::Pin pinNumber) {
	if (!socket)
		mainboard->panic(Exceptions::ERR_INVALID_SOCKET);
	if (pinNumber < 3 || pinNumber > 9)
		mainboard->panic(Exceptions::ERR_PIN_OUT_OF_RANGE);

	this->cpuPin = socket->pins[pinNumber];
	this->resistorMode = static_cast<ResistorMode>(0xFF);
	this->ioState = static_cast<IOState>(0xFF);
}

DigitalIO::DigitalIO(CPUPin pin) {
	this->cpuPin = pin;
	
	this->resistorMode = static_cast<ResistorMode>(0xFF);
	this->ioState = static_cast<IOState>(0xFF);
}

DigitalIO::~DigitalIO() {

}

void DigitalIO::write(bool value) {
	this->setIOState(IOStates::DIGITAL_OUTPUT);
	mainboard->writeDigital(this->cpuPin, value);
}

bool DigitalIO::read() {
	this->setIOState(IOStates::DIGITAL_INPUT);
	return mainboard->readDigital(this->cpuPin);
}

void DigitalIO::setIOState(IOState state) {
	if (this->ioState == state)
		return;
	
	this->ioState = state;
	mainboard->setIOMode(this->cpuPin, this->ioState, this->resistorMode);
}

void DigitalIO::setResistorMode(ResistorMode mode) {
	if (this->resistorMode == mode)
		return;
	
	this->resistorMode = mode;
	mainboard->setIOMode(this->cpuPin, this->ioState, this->resistorMode);
}

ResistorMode DigitalIO::getResistorMode() {
	return this->resistorMode;
}

IOState DigitalIO::getIOState() {
	return this->ioState;
}

AnalogInput::AnalogInput(Socket* socket, Socket::Pin pinNumber) {
	if (!socket)
		mainboard->panic(Exceptions::ERR_INVALID_SOCKET);
	if (!socket || pinNumber < 3 || pinNumber > 9)
		mainboard->panic(Exceptions::ERR_PIN_OUT_OF_RANGE);

	this->cpuPin = socket->pins[pinNumber];
}

AnalogInput::AnalogInput(CPUPin pin) {
	this->cpuPin = pin;
}

AnalogInput::~AnalogInput() {

}

double AnalogInput::read() {
	return mainboard->readAnalog(this->cpuPin);
}

double AnalogInput::readProportion() {
	return mainboard->readAnalogProportion(this->cpuPin);
}

AnalogOutput::AnalogOutput(Socket* socket, Socket::Pin pinNumber) {
	if (!socket)
		mainboard->panic(Exceptions::ERR_INVALID_SOCKET);
	if (!socket || pinNumber < 3 || pinNumber > 9)
		mainboard->panic(Exceptions::ERR_PIN_OUT_OF_RANGE);

	this->cpuPin = socket->pins[pinNumber];
}

AnalogOutput::AnalogOutput(CPUPin pin) {
	this->cpuPin = pin;
}

AnalogOutput::~AnalogOutput() {

}

void AnalogOutput::write(double value) {
	mainboard->writeAnalog(this->cpuPin, value);
}

void AnalogOutput::writeProportion(double value) {
	mainboard->writeAnalogProportion(this->cpuPin, value);
}

PWMOutput::PWMOutput(Socket* socket, Socket::Pin pinNumber) {
	if (!socket)
		mainboard->panic(Exceptions::ERR_INVALID_SOCKET);
	if (pinNumber < 3 || pinNumber > 9)
		mainboard->panic(Exceptions::ERR_PIN_OUT_OF_RANGE);

	this->cpuPin = socket->pins[pinNumber];
		
	mainboard->setIOMode(this->cpuPin, IOStates::PWM);

	this->set(0, 0);
}

PWMOutput::PWMOutput(CPUPin pin) {
	this->cpuPin = pin;
		
	mainboard->setIOMode(this->cpuPin, IOStates::PWM);

	this->set(0, 0);
}

void PWMOutput::set(double frequency, double dutyCycle) {
	this->dutyCycle = dutyCycle;
	this->frequency = frequency;

	mainboard->setPWM(this->cpuPin, this->frequency, this->dutyCycle);
}

PWMOutput::~PWMOutput() {

}

void PWMOutput::setFrequency(double frequency) {
	this->set(frequency, this->dutyCycle);
}

void PWMOutput::setDutyCycle(double dutyCycle) {
	this->set(this->frequency, dutyCycle);
}