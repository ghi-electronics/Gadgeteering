#include "Thermocouple.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

Thermocouple::Thermocouple(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::X);
	
    _miso = new DigitalInput(socket, Socket::Pins::Three, ResistorModes::PULL_UP);
    _clk = new DigitalOutput(socket, Socket::Pins::Four, false);
    _cs = new DigitalOutput(socket, Socket::Pins::Five, true);
}

Thermocouple::~Thermocouple() {
	delete this->_miso;
	delete this->_clk;
	delete this->_cs;
}

unsigned long Thermocouple::ReadData()
{
    long bitCount;
    unsigned long data = 0;

    _cs->write(false);
    {
        for (bitCount = 31; bitCount >= 0; bitCount--)
        {
            _clk->write(true);

            if (_miso->read())
            {
                data |= (unsigned long)(1 << bitCount);
            }

            _clk->write(false);
        }
    }
    _cs->write(true);

	return data;
}

short Thermocouple::GetExternalTemp_Celsius()
{
	return (ReadData() >> 20) & 0xFFF;
}

short Thermocouple::GetExternalTemp_Fahrenheit()
{
    return (short)((GetExternalTemp_Celsius() * 1.8) + 32);
}

unsigned char Thermocouple::GetInternalTemp_Celsius()
{
    unsigned long value = ReadData();
    return (unsigned char)((value >> 8) & 0xFF); // get byte 2
}
