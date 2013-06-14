#include "Load.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

Load::Load(int socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::Y);
	
    this->P1 = new DigitalOutput(socket, Socket::Pins::Three, false);
    this->P2 = new DigitalOutput(socket, Socket::Pins::Four, false);
    this->P3 = new DigitalOutput(socket, Socket::Pins::Five, false);
    this->P4 = new DigitalOutput(socket, Socket::Pins::Six, false);
    this->P5 = new DigitalOutput(socket, Socket::Pins::Seven, false);
    this->P6 = new DigitalOutput(socket, Socket::Pins::Eight, false);
    this->P7 = new DigitalOutput(socket, Socket::Pins::Nine, false);
}

Load::~Load() {
    delete this->P1;
    delete this->P2;
    delete this->P3;
    delete this->P4;
    delete this->P5;
    delete this->P6;
    delete this->P7;
}