#include "CurrentACS712.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

CurrentACS712::CurrentACS712(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::A);

    this->ain = new AnalogInput(socket, Socket::Pins::Five);
}

CurrentACS712::~CurrentACS712() {
	delete this->ain;
}

double CurrentACS712::Read_AC_Current()
{
    double read = 0.0;
    double calculation = 0.0;

    for (int i = 0; i < CurrentACS712::AC_SAMPLE_COUNT; i++)
    {
        read += ain->readProportion();
    }

    read /= CurrentACS712::AC_SAMPLE_COUNT;
    calculation = 21.2 * read - 13.555;

    return calculation;
}

double CurrentACS712::Read_DC_Current()
{
    double read = 0.0;
    double calculation = 0.0;

    for (int i = 0; i < CurrentACS712::AC_SAMPLE_COUNT; i++)
    {
        read = ain->readProportion();
        read = 21.3 * read - 13.555;
        read = read < 0 ? read * -1 : read;

        if (calculation < read)
            calculation = read;
    }

    calculation /= 1.41421356;

    return calculation;
}