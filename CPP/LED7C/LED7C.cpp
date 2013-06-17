#include "LED7C.h"
#include "../Gadgeteering/System.hpp"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

LED7C::LED7C(unsigned char socketNumber) 
{
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::X);
	
	this->red = new DigitalOutput(socket, Socket::Pins::Four);
	this->green = new DigitalOutput(socket, Socket::Pins::Five);
	this->blue = new DigitalOutput(socket, Socket::Pins::Three);
}

LED7C::~LED7C() 
{
	delete this->red;
	delete this->green;
	delete this->blue;
}

void LED7C::setColor(Color color) {
	int r = ((int)(color) & 4);

    this->red->write((((int)color & 4) != 0 ? true : false));
    this->green->write((((int)color & 2) != 0 ? true : false));
    this->blue->write((((int)color & 1) != 0 ? true : false));
}
