#include "MotionSensor.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

MotionSensor::MotionSensor(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::X);

    this->input = new DigitalInput(socket, Socket::Pins::Three, ResistorModes::PULL_UP);
}

MotionSensor::~MotionSensor() {
	delete this->input;
}

bool MotionSensor::isMotionDetected() {
	return this->input->read();
}
