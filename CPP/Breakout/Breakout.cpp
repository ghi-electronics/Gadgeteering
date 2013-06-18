#include "Breakout.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

Breakout::Breakout(unsigned char socketNumber) {
	this->socket = mainboard->getSocket(socketNumber);
}

Breakout::~Breakout() {

}
				
Interfaces::DigitalInput* Breakout::SetupDigitalInput(Socket::Pin pin, ResistorMode resistorMode) {
	return new Interfaces::DigitalInput(this->socket, pin, resistorMode);
}

Interfaces::DigitalOutput* Breakout::SetupDigitalOutput(Socket::Pin pin, bool initialState) {
	return new Interfaces::DigitalOutput(this->socket, pin, initialState);
}

Interfaces::DigitalIO* Breakout::SetupDigitalIO(Socket::Pin pin) {
	return new Interfaces::DigitalIO(this->socket, pin);
}

Interfaces::AnalogInput* Breakout::SetupAnalogInput(Socket::Pin pin) {
	return new Interfaces::AnalogInput(this->socket, pin);
}

Interfaces::AnalogOutput* Breakout::SetupAnalogOutput(Socket::Pin pin) {
	return new Interfaces::AnalogOutput(this->socket, pin);
}

Interfaces::PWMOutput* Breakout::SetupPWMOutput(Socket::Pin pin) {
	return new Interfaces::PWMOutput(this->socket, pin);
}
