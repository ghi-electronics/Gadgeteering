#include "LED7R.h"
#include "../Gadgeteering/System.hpp"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

LED7R::LED7R(int socketNumber) 
{
	this->socket = mainboard->getSocket(socketNumber);
	this->socket->ensureTypeIsSupported(Socket::Types::Y);

	for (int i = 0; i < LED7R::LEDS; i++)
		this->ports[i] = new DigitalOutput(this->socket, i + 3, false);
}

void LED7R::turnOnLED(int led, bool onlyLED) {
	if (led < 1 || led > LED7R::LEDS)
		mainboard->panic("led out of range");

	if (onlyLED)
		this->turnAllOff();
			
	this->ports[led - 1]->write(true);
}

void LED7R::turnOffLED(int led) {
	if (led < 1 || led > 7)
		mainboard->panic("led out of range");

	this->ports[led - 1]->write(false);
}

void LED7R::turnAllOn() {
	for (int i = 0; i < LED7R::LEDS; i++)
		this->ports[i]->write(true);
}

void LED7R::turnAllOff() {
	for (int i = 0; i < LED7R::LEDS; i++)
		this->ports[i]->write(false);
}

void LED7R::animate(int switchTime, bool clockwise, bool turnOn, bool remainOn) {
	int length = 7;
	int i;
	int terminate;
	int dir;

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
