#include "LEDStrip.h"
#include "../Gadgeteering/System.hpp"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

LEDStrip::LEDStrip(unsigned char socketNumber) 
{
	this->socket = mainboard->getSocket(socketNumber);
	this->socket->ensureTypeIsSupported(Socket::Types::Y);
	
	for (unsigned char i = 0; i < LEDStrip::LEDS; i++)
		this->ports[i] = new DigitalOutput(this->socket, i + 3, false);
}

LEDStrip::~LEDStrip() 
{
	for (unsigned char i = 0; i < LEDStrip::LEDS; i++)
		delete this->ports[i];
}

void LEDStrip::turnOnLED(unsigned char led, bool onlyLED) {
	if (led < 1 || led > LEDStrip::LEDS)
		mainboard->panic(ERR_MODULE_ERROR);

	if (onlyLED)
		this->turnAllOff();
			
	this->ports[led - 1]->write(true);
}

void LEDStrip::turnOffLED(unsigned char led) {
	if (led < 1 || led > 7)
		mainboard->panic(ERR_MODULE_ERROR);

	this->ports[led - 1]->write(false);
}

void LEDStrip::turnAllOn() {
	for (unsigned char i = 0; i < LEDStrip::LEDS; i++)
		this->ports[i]->write(true);
}

void LEDStrip::turnAllOff() {
	for (unsigned char i = 0; i < LEDStrip::LEDS; i++)
		this->ports[i]->write(false);
}

void LEDStrip::animate(unsigned int switchTime, bool clockwise, bool turnOn, bool remainOn) {
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
