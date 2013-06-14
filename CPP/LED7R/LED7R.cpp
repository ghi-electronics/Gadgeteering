#include "LED7R.h"
#include "../Gadgeteering/System.hpp"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

LED7R::LED7R(unsigned char socketNumber) 
{
	this->socket = mainboard->getSocket(socketNumber);
	this->socket->ensureTypeIsSupported(Socket::Types::Y);
	
	for (unsigned char i = 0; i < LED7R::LEDS; i++)
		this->ports[i] = new DigitalOutput(this->socket, i + 3, false);
}

LED7R::~LED7R() 
{
	for (unsigned char i = 0; i < LED7R::LEDS; i++)
		delete this->ports[i];
}

void LED7R::turnOnLED(unsigned char led, bool onlyLED) {
	if (led < 1 || led > LED7R::LEDS)
		mainboard->panic(ERR_MODULE_ERROR);

	if (onlyLED)
		this->turnAllOff();
			
	this->ports[led - 1]->write(true);
}

void LED7R::turnOffLED(unsigned char led) {
	if (led < 1 || led > 7)
		mainboard->panic(ERR_MODULE_ERROR);

	this->ports[led - 1]->write(false);
}

void LED7R::turnAllOn() {
	for (unsigned char i = 0; i < LED7R::LEDS; i++)
		this->ports[i]->write(true);
}

void LED7R::turnAllOff() {
	for (unsigned char i = 0; i < LED7R::LEDS; i++)
		this->ports[i]->write(false);
}

void LED7R::animate(unsigned int switchTime, bool clockwise, bool turnOn, bool remainOn) {
	int length = 7;

	if (clockwise) {
		for (int i = 1; i <= 7; i++) {
			if (turnOn)
				this->turnOnLED(i, !remainOn);
			else
				this->turnOffLED(i);

			GHI::System::Sleep(switchTime);
		}
	}
	else {
		for (int i = 7; i >= 1; i--) {
			if (turnOn)
				this->turnOnLED(i, !remainOn);
			else
				this->turnOffLED(i);
			
			GHI::System::Sleep(switchTime);
		}
	}
}
