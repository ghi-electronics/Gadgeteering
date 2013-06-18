#include "IO60P16.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

IO60P16::IO60P16(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::X);
	I2CBus* bus = mainboard->getI2CBus(socket);
	this->io60Chip = new I2CDevice(bus, 0x20);
}

IO60P16::~IO60P16() {
	delete this->io60Chip;
}

unsigned char IO60P16::getPort(CPUPin pin) {
	return pin >> 4;
}

unsigned char IO60P16::getMask(CPUPin pin) {
	return 1 << (pin & 0x0F);
}

void IO60P16::setIOMode(CPUPin pinNumber, IOState state, ResistorMode resistorMode) {
	this->io60Chip->writeRegister(IO60P16::PORT_SELECT_REGISTER, this->getPort(pinNumber));
	
	unsigned char mask = this->getMask(pinNumber);
	unsigned char val = this->io60Chip->readRegister(IO60P16::ENABLE_PWM_REGISTER);

	if (state == IOStates::PWM)	{
		this->io60Chip->writeRegister(IO60P16::ENABLE_PWM_REGISTER, val | mask);
		
		this->writeDigital(pinNumber, true);

		this->io60Chip->writeRegister(IO60P16::PWM_SELECT_REGISTER, (unsigned char)((pinNumber % 8) + (this->getPort(pinNumber) - 6) * 8));
		this->io60Chip->writeRegister(IO60P16::PWM_CONFIG, IO60P16::CLOCK_SOURCE); //93.75KHz clock
	}
	else {
		this->io60Chip->writeRegister(IO60P16::ENABLE_PWM_REGISTER, val & ~mask);
		val = this->io60Chip->readRegister(IO60P16::PIN_DIRECTION_REGISTER);

		if (state == IOStates::DIGITAL_INPUT) {
			this->io60Chip->writeRegister(IO60P16::PIN_DIRECTION_REGISTER, val | mask);

			unsigned char resistorRegister = IO60P16::PIN_HIGH_IMPEDENCE;
			if (resistorMode == ResistorModes::PULL_DOWN)
				resistorRegister = IO60P16::PIN_PULL_DOWN;
			else if (resistorMode == ResistorModes::PULL_UP)
				resistorRegister = IO60P16::PIN_PULL_UP;

			val = this->io60Chip->readRegister(resistorRegister);
			this->io60Chip->writeRegister(resistorRegister, val | mask);
		}
		else {
			this->io60Chip->writeRegister(IO60P16::PIN_DIRECTION_REGISTER, val & ~mask);
			
			val = this->io60Chip->readRegister(IO60P16::PIN_STRONG_DRIVE);
			this->io60Chip->writeRegister(IO60P16::PIN_STRONG_DRIVE, val | mask);
		}
	}
}

//We're using the 93.75KHz clock source because it gives a good resolution around the 1KHz frequency
//while still allowing the user to select frequencies such as 10KHz, but with reduced duty cycle
//resolution.
void IO60P16::setPWM(CPUPin pin, double frequency, double dutyCycle) {
	this->io60Chip->writeRegister(IO60P16::PWM_SELECT_REGISTER, (char)((pin % 8) + (this->getPort(pin) - 6) * 8));
	
	unsigned char period = (unsigned char)(93750 / frequency);

	this->io60Chip->writeRegister(IO60P16::PERIOD_REGISTER, period);
	this->io60Chip->writeRegister(IO60P16::PULSE_WIDTH_REGISTER, period * dutyCycle);
}

bool IO60P16::readDigital(CPUPin pin) {
	unsigned char b = this->io60Chip->readRegister(IO60P16::INPUT_PORT_0_REGISTER + this->getPort(pin));

	return b & this->getMask(pin);
}

void IO60P16::writeDigital(CPUPin pin, bool value) {
	unsigned char b = this->io60Chip->readRegister(IO60P16::OUTPUT_PORT_0_REGISTER + this->getPort(pin));

	if (value)
		b |= this->getMask(pin);
	else
		b &= ~this->getMask(pin);
	
	this->io60Chip->writeRegister(IO60P16::OUTPUT_PORT_0_REGISTER + this->getPort(pin), b);
}