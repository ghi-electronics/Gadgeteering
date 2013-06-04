#include "ExtenderChip.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

ExtenderChip::ExtenderChip(Socket::Pin sdaPin, Socket::Pin sclPin, byte address) {
	Socket* socket = new Socket(-1, Socket::Types::I);
	socket->pins[8] = sdaPin;
	socket->pins[9] = sclPin;

	this->io60Chip = new SoftwareI2C(0x20, socket);
}

ExtenderChip::~ExtenderChip() {
	delete this->io60Chip;
}

byte ExtenderChip::getPort(Socket::Pin pin) {
	return pin >> 4;
}

byte ExtenderChip::getPin(Socket::Pin pin) {
	return 1 << (pin & 0x0F);
}

void ExtenderChip::setIOMode(Socket::Pin pin, Mainboard::IOState state, Mainboard::ResistorMode resistorMode) {
	this->io60Chip->writeRegister(ExtenderChip::PORT_SELECT_REGISTER, this->getPort(pin));
	byte pin = this->getPin(pin);
	byte val = this->io60Chip->readRegister(ExtenderChip::ENABLE_PWM_REGISTER);

	if (state == Mainboard::IOStates::PWM)	{
		this->io60Chip->writeRegister(ExtenderChip::ENABLE_PWM_REGISTER, val | pin);

		this->io60Chip->writeRegister(ExtenderChip::PWM_CONFIG, ExtenderChip::CLOCK_SOURCE); //93.75KHz clock

		this->writeDigital(pin, true);
	}
	else {
		this->io60Chip->writeRegister(ExtenderChip::ENABLE_PWM_REGISTER, val & ~pin);
		val = this->io60Chip->readRegister(ExtenderChip::PIN_DIRECTION_REGISTER);

		if (state == Mainboard::IOStates::IN) {
			byte resistorRegister = ExtenderChip::PIN_HIGH_IMPEDENCE;
			if (resistorMode == Mainboard::ResistorModes::PULL_DOWN)
				resistorRegister = ExtenderChip::PIN_PULL_DOWN;
			else if (resistorMode == Mainboard::ResistorModes::PULL_UP)
				resistorRegister = ExtenderChip::PIN_PULL_UP;

			this->io60Chip->writeRegister(ExtenderChip::PIN_DIRECTION_REGISTER, val | pin);
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
void ExtenderChip::setPWM(Socket::Pin pin, double frequency, double dutyCycle) {
	this->io60Chip->writeRegister(ExtenderChip::PWM_SELECT_REGISTER, (byte)((pin % 8) + (this->getPort(pin) - 6) * 8));
	
	byte period = (byte)(93750 / frequency);

	this->io60Chip->writeRegister(ExtenderChip::PERIOD_REGISTER, period);
	this->io60Chip->writeRegister(ExtenderChip::PULSE_WIDTH_REGISTER, period * dutyCycle);
}

bool ExtenderChip::readDigital(Socket::Pin pin) {
	byte b = this->io60Chip->readRegister(ExtenderChip::INPUT_PORT_0_REGISTER + this->getPort(pin));

	return b & this->getPin(pin);
}

void ExtenderChip::writeDigital(Socket::Pin pin, bool value) {
	byte b = this->io60Chip->readRegister(ExtenderChip::OUTPUT_PORT_0_REGISTER + this->getPort(pin));

	if (value)
		b |= this->getPin(pin);
	else
		b &= ~this->getPin(pin);

	this->io60Chip->writeRegister(ExtenderChip::OUTPUT_PORT_0_REGISTER + this->getPort(pin), b);
}

double ExtenderChip::readAnalog(Socket::Pin pin) {
	mainboard->panic("Not supported");
}

void ExtenderChip::writeAnalog(Socket::Pin pin, double voltage) {
	mainboard->panic("Not supported");
}
