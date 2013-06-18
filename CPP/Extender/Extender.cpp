#include "Extender.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

Extender::Extender(unsigned char socketNumber) {
	this->socket = mainboard->getSocket(socketNumber);
}

Extender::~Extender() {

}
				
Interfaces::DigitalInput* Extender::SetupDigitalInput(Socket::Pin pin, ResistorMode resistorMode) {
	return new Interfaces::DigitalInput(this->socket, pin, resistorMode);
}

Interfaces::DigitalOutput* Extender::SetupDigitalOutput(Socket::Pin pin, bool initialState) {
	return new Interfaces::DigitalOutput(this->socket, pin, initialState);
}

Interfaces::DigitalIO* Extender::SetupDigitalIO(Socket::Pin pin) {
	return new Interfaces::DigitalIO(this->socket, pin);
}

Interfaces::AnalogInput* Extender::SetupAnalogInput(Socket::Pin pin) {
	return new Interfaces::AnalogInput(this->socket, pin);
}

Interfaces::AnalogOutput* Extender::SetupAnalogOutput(Socket::Pin pin) {
	return new Interfaces::AnalogOutput(this->socket, pin);
}

Interfaces::PWMOutput* Extender::SetupPWMOutput(Socket::Pin pin) {
	return new Interfaces::PWMOutput(this->socket, pin);
}
