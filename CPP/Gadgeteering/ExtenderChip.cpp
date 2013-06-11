#include "ExtenderChip.hpp"
#include "SoftwareI2C.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

ExtenderChip::ExtenderChip(CPUPin sdaPin, CPUPin sclPin, char address) {
	this->io60Chip = new SoftwareI2C(address, sdaPin, sclPin);
}

ExtenderChip::~ExtenderChip() {
	delete this->io60Chip;
}

char ExtenderChip::getPort(CPUPin pin) {
	return pin >> 4;
}

char ExtenderChip::getPin(CPUPin pin) {
	return 1 << (pin & 0x0F);
}

void ExtenderChip::setIOMode(CPUPin pinNumber, IOState state, ResistorMode resistorMode) {
	this->io60Chip->writeRegister(ExtenderChip::PORT_SELECT_REGISTER, this->getPort(pinNumber));

	char pin = this->getPin(pinNumber);
	unsigned char val = this->io60Chip->readRegister(ExtenderChip::ENABLE_PWM_REGISTER);

	if (state == IOStates::PWM)	{
		this->io60Chip->writeRegister(ExtenderChip::ENABLE_PWM_REGISTER, val | pin);
		
		this->writeDigital(pinNumber, true);

		this->io60Chip->writeRegister(ExtenderChip::PWM_SELECT_REGISTER, (char)((pinNumber % 8) + (this->getPort(pinNumber) - 6) * 8));
		this->io60Chip->writeRegister(ExtenderChip::PWM_CONFIG, ExtenderChip::CLOCK_SOURCE); //93.75KHz clock
	}
	else {
		this->io60Chip->writeRegister(ExtenderChip::ENABLE_PWM_REGISTER, val & ~pin);
		val = this->io60Chip->readRegister(ExtenderChip::PIN_DIRECTION_REGISTER);

		if (state == IOStates::DIGITAL_INPUT) {
			this->io60Chip->writeRegister(ExtenderChip::PIN_DIRECTION_REGISTER, val | pin);

			char resistorRegister = ExtenderChip::PIN_HIGH_IMPEDENCE;
			if (resistorMode == ResistorModes::PULL_DOWN)
				resistorRegister = ExtenderChip::PIN_PULL_DOWN;
			else if (resistorMode == ResistorModes::PULL_UP)
				resistorRegister = ExtenderChip::PIN_PULL_UP;

			val = this->io60Chip->readRegister(resistorRegister);
			this->io60Chip->writeRegister(resistorRegister, val | pin);
		}
		else {
			this->io60Chip->writeRegister(ExtenderChip::PIN_DIRECTION_REGISTER, val & ~pin);
			
			val = this->io60Chip->readRegister(ExtenderChip::PIN_STRONG_DRIVE);
			this->io60Chip->writeRegister(ExtenderChip::PIN_STRONG_DRIVE, val | pin);
		}
	}
}

//We're using the 93.75KHz clock source because it gives a good resolution around the 1KHz frequency
//while still allowing the user to select frequencies such as 10KHz, but with reduced duty cycle
//resolution.
void ExtenderChip::setPWM(CPUPin pin, double frequency, double dutyCycle) {
	this->io60Chip->writeRegister(ExtenderChip::PWM_SELECT_REGISTER, (char)((pin % 8) + (this->getPort(pin) - 6) * 8));
	
	char period = (char)(93750 / frequency);

	this->io60Chip->writeRegister(ExtenderChip::PERIOD_REGISTER, period);
	this->io60Chip->writeRegister(ExtenderChip::PULSE_WIDTH_REGISTER, period * dutyCycle);
}

bool ExtenderChip::readDigital(CPUPin pin) {
	char b = this->io60Chip->readRegister(ExtenderChip::INPUT_PORT_0_REGISTER + this->getPort(pin));

	return b & this->getPin(pin);
}

void ExtenderChip::writeDigital(CPUPin pin, bool value) {
	unsigned char b = this->io60Chip->readRegister(ExtenderChip::OUTPUT_PORT_0_REGISTER + this->getPort(pin));

	if (value)
		b |= this->getPin(pin);
	else
		b &= ~this->getPin(pin);

	this->io60Chip->writeRegister(ExtenderChip::OUTPUT_PORT_0_REGISTER + this->getPort(pin), b);
}

double ExtenderChip::readAnalog(CPUPin pin) {
	mainboard->panic("Not supported");

	return 0.0;
}

void ExtenderChip::writeAnalog(CPUPin pin, double voltage) {
	mainboard->panic("Not supported");

}
