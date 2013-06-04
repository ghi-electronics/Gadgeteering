#include "LED7R.h"

LED7R::LED7R(int socketNumber) {
	this->socket = mainboard->getSocket(socketNumber);
	this->socket->ensureTypeIsSupported(Socket::Types::Y);

	for (int i = 0; i < LED7R::LEDS; i++)
		this->ports[i] = new DigitalOutput(this->socket, i + 3, false);
}

void LED7R::turnOnLED(int led, bool onlyLED) {
	if (led < 1 || led > LED7R::LEDS)
		mainboard->panic();

	if (onlyLED)
		this->turnAllOff();
			
	this->ports[led - 1]->write(true);
}

void LED7R::turnOffLED(int led) {
	if (led < 1 || led > 7)
		mainboard->panic();

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
