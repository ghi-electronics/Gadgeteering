#include "ExtenderChip.hpp"
#include "I2CBus.hpp"
#include "Mainboard.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

ExtenderChip::ExtenderChip(CPUPin sda, CPUPin scl) {
	I2CBus* bus = mainboard->getNewI2CBus(sda, scl);
	this->io60Chip = new I2CDevice(bus, 0x20);
}

ExtenderChip::~ExtenderChip() {
	delete this->io60Chip;
}

unsigned char ExtenderChip::getPort(CPUPin pin) {
	return pin >> 4;
}

unsigned char ExtenderChip::getMask(CPUPin pin) {
	return 1 << (pin & 0x0F);
}

void ExtenderChip::setIOMode(CPUPin pinNumber, IOState state, ResistorMode resistorMode) {
	this->io60Chip->writeRegister(ExtenderChip::PORT_SELECT_REGISTER, this->getPort(pinNumber));

	unsigned char mask = this->getMask(pinNumber);
	unsigned char val = this->io60Chip->readRegister(ExtenderChip::ENABLE_PWM_REGISTER);

	if (state == IOStates::PWM)	{
		this->io60Chip->writeRegister(ExtenderChip::ENABLE_PWM_REGISTER, val | mask);
		
		this->writeDigital(pinNumber, true);

		this->io60Chip->writeRegister(ExtenderChip::PWM_SELECT_REGISTER, (unsigned char)((pinNumber % 8) + (this->getPort(pinNumber) - 6) * 8));
		this->io60Chip->writeRegister(ExtenderChip::PWM_CONFIG, ExtenderChip::CLOCK_SOURCE); //93.75KHz clock
	}
	else {
		this->io60Chip->writeRegister(ExtenderChip::ENABLE_PWM_REGISTER, val & ~mask);
		val = this->io60Chip->readRegister(ExtenderChip::PIN_DIRECTION_REGISTER);

		if (state == IOStates::DIGITAL_INPUT) {
			this->io60Chip->writeRegister(ExtenderChip::PIN_DIRECTION_REGISTER, val | mask);

			unsigned char resistorRegister = ExtenderChip::PIN_HIGH_IMPEDENCE;
			if (resistorMode == ResistorModes::PULL_DOWN)
				resistorRegister = ExtenderChip::PIN_PULL_DOWN;
			else if (resistorMode == ResistorModes::PULL_UP)
				resistorRegister = ExtenderChip::PIN_PULL_UP;

			val = this->io60Chip->readRegister(resistorRegister);
			this->io60Chip->writeRegister(resistorRegister, val | mask);
		}
		else {
			this->io60Chip->writeRegister(ExtenderChip::PIN_DIRECTION_REGISTER, val & ~mask);
			
			val = this->io60Chip->readRegister(ExtenderChip::PIN_STRONG_DRIVE);
			this->io60Chip->writeRegister(ExtenderChip::PIN_STRONG_DRIVE, val | mask);
		}
	}
}

//We're using the 93.75KHz clock source because it gives a good resolution around the 1KHz frequency
//while still allowing the user to select frequencies such as 10KHz, but with reduced duty cycle
//resolution.
void ExtenderChip::setPWM(CPUPin pin, double frequency, double dutyCycle) {
	this->io60Chip->writeRegister(ExtenderChip::PWM_SELECT_REGISTER, (char)((pin % 8) + (this->getPort(pin) - 6) * 8));
	
	unsigned char period = (unsigned char)(93750 / frequency);

	this->io60Chip->writeRegister(ExtenderChip::PERIOD_REGISTER, period);
	this->io60Chip->writeRegister(ExtenderChip::PULSE_WIDTH_REGISTER, period * dutyCycle);
}

bool ExtenderChip::readDigital(CPUPin pin) {
	unsigned char b = this->io60Chip->readRegister(ExtenderChip::INPUT_PORT_0_REGISTER + this->getPort(pin));

	return b & this->getMask(pin);
}

void ExtenderChip::writeDigital(CPUPin pin, bool value) {
	unsigned char b = this->io60Chip->readRegister(ExtenderChip::OUTPUT_PORT_0_REGISTER + this->getPort(pin));

	if (value)
		b |= this->getMask(pin);
	else
		b &= ~this->getMask(pin);

	this->io60Chip->writeRegister(ExtenderChip::OUTPUT_PORT_0_REGISTER + this->getPort(pin), b);
}