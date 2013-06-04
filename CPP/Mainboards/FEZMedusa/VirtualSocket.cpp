#include "VirtualSocket.hpp"

SoftwareI2C* VirtualSocket::extenderChip = NULL;

void VirtualSocket::initExtenderChip(Pin sdaPin, Pin sclPin, byte address) {
	Socket* socket = new Socket(-1, Socket::Types::I);
	socket->pins[8] = sdaPin;
	socket->pins[9] = sclPin;
	VirtualSocket::extenderChip = new SoftwareI2C(address, socket);
}

VirtualSocket::VirtualSocket(int number, Type type) : Socket(number, type) {

}

byte VirtualSocket::getPort(Pin pinNumber) {
	return this->pins[pinNumber] >> 4;
}

byte VirtualSocket::getPin(Pin pinNumber) {
	return 1 << (this->pins[pinNumber] & 0x0F);
}

void VirtualSocket::setIOMode(Pin pinNumber, IOState state, ResistorMode resistorMode) {
	VirtualSocket::extenderChip->writeRegister(VirtualSocket::PORT_SELECT_REGISTER, this->getPort(pinNumber));
	byte pin = this->getPin(pinNumber);
	byte val = VirtualSocket::extenderChip->readRegister(VirtualSocket::ENABLE_PWM_REGISTER);

	if (state == IOStates::PWM)	{
		VirtualSocket::extenderChip->writeRegister(VirtualSocket::ENABLE_PWM_REGISTER, val | pin);

		VirtualSocket::extenderChip->writeRegister(VirtualSocket::PWM_CONFIG, VirtualSocket::CLOCK_SOURCE); //93.75KHz clock

		this->writeDigital(pinNumber, true);
	}
	else {
		VirtualSocket::extenderChip->writeRegister(VirtualSocket::ENABLE_PWM_REGISTER, val & ~pin);
		val = VirtualSocket::extenderChip->readRegister(VirtualSocket::PIN_DIRECTION_REGISTER);

		if (state == IOStates::IN) {
			byte resistorRegister = VirtualSocket::PIN_HIGH_IMPEDENCE;
			if (resistorMode == ResistorModes::PULL_DOWN)
				resistorRegister = VirtualSocket::PIN_PULL_DOWN;
			else if (resistorMode == ResistorModes::PULL_UP)
				resistorRegister = VirtualSocket::PIN_PULL_UP;

			VirtualSocket::extenderChip->writeRegister(VirtualSocket::PIN_DIRECTION_REGISTER, val | pin);
			val = VirtualSocket::extenderChip->readRegister(resistorRegister);
			VirtualSocket::extenderChip->writeRegister(resistorRegister, val | pin);
		}
		else {
			VirtualSocket::extenderChip->writeRegister(VirtualSocket::PIN_DIRECTION_REGISTER, val & ~pin);
			val = VirtualSocket::extenderChip->readRegister(VirtualSocket::PIN_STRONG_DRIVE);
			VirtualSocket::extenderChip->writeRegister(VirtualSocket::PIN_STRONG_DRIVE, val | pin);
		}
	}
}

//We're using the 93.75KHz clock source because it gives a good resolution around the 1KHz frequency
//while still allowing the user to select frequencies such as 10KHz, but with reduced duty cycle
//resolution.
void VirtualSocket::setPWM(Pin pinNumber, double frequency, double dutyCycle) {
	VirtualSocket::extenderChip->writeRegister(VirtualSocket::PWM_SELECT_REGISTER, (byte)((this->pins[pinNumber] % 8) + (this->getPort(pinNumber) - 6) * 8));
	
	byte period = (byte)(93750 / frequency);

	VirtualSocket::extenderChip->writeRegister(VirtualSocket::PERIOD_REGISTER, period);
	VirtualSocket::extenderChip->writeRegister(VirtualSocket::PULSE_WIDTH_REGISTER, period * dutyCycle);
}

bool VirtualSocket::readDigital(Pin pinNumber) {
	byte b = VirtualSocket::extenderChip->readRegister(VirtualSocket::INPUT_PORT_0_REGISTER + this->getPort(pinNumber));

	return b & this->getPin(pinNumber);
}

void VirtualSocket::writeDigital(Pin pinNumber, bool value) {
	byte b = VirtualSocket::extenderChip->readRegister(VirtualSocket::OUTPUT_PORT_0_REGISTER + this->getPort(pinNumber));

	if (value)
		b |= this->getPin(pinNumber);
	else
		b &= ~this->getPin(pinNumber);

	VirtualSocket::extenderChip->writeRegister(VirtualSocket::OUTPUT_PORT_0_REGISTER + this->getPort(pinNumber), b);
}

double VirtualSocket::readAnalog(Pin pinNumber) {
	mainboard->panic("Not supported");
}

void VirtualSocket::writeAnalog(Pin pinNumber, double voltage) {
	mainboard->panic("Not supported");
}
