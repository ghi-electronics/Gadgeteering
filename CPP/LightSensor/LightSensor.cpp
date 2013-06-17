#include "LightSensor.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

LightSensor::LightSensor(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::A);
	
    this->input = new AnalogInput(socket, Socket::Pins::Three);
}

LightSensor::~LightSensor() {
    delete this->input;
}

double LightSensor::ReadLightSensorVoltage()
{
    return input->read();
}

double LightSensor::ReadLightSensorPercentage()
{
    return (input->read() / 3.3 * 100);
}
