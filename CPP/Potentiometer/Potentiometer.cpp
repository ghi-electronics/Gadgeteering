#include "Potentiometer.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

Potentiometer::Potentiometer(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::A);
	
    this->input = new AnalogInput(socket, Socket::Pins::Three);
}

Potentiometer::~Potentiometer() {
    delete this->input;
}

double Potentiometer::ReadPotentiometerVoltage()
{
    return input->read();
}

double Potentiometer::ReadPotentiometerPercentage()
{
    return (input->read() / 3.3 * 100);
}
