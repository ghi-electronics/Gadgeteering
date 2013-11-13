/*
Copyright 2013 GHI Electronics LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "IO60P16.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

io60p16::io60p16(unsigned char socketNumber) {
	const socket& s = mainboard->get_socket(socketNumber);

	//s.ensure_type(socket::types::X);
	//fix once hubap5 driver is done

	this->io60Chip = new devices::i2c(s.i2c, 0x20);

	for(int i = 0; i < 8; i++)
	{
		this->outputPorts[i] = 0xFF;
		this->pinDirections[i] = 0x00;
		this->pwms[i] = 0x00;
		this->resistors[0][i] = 0xFF;
	}
	
	for(int i = 1; i < 4; i++)
		for(int j = 0; j < 8; j++)
			this->resistors[i][j] = 0x00;
}

io60p16::~io60p16() {
	delete this->io60Chip;
}

void io60p16::changeResistor(unsigned char port, unsigned char mask, unsigned char newResistor) {
	for (unsigned int i = 0; i < 4; i++)
		this->resistors[i][port] &= ~mask;

	unsigned int offset = 0;
	switch (newResistor) {
		case io60p16::PIN_PULL_UP: offset = 0; break;
		case io60p16::PIN_PULL_DOWN: offset = 1; break;
		case io60p16::PIN_HIGH_IMPEDENCE: offset = 2; break;
		case io60p16::PIN_STRONG_DRIVE: offset = 3; break;
	}

	this->resistors[offset][port] |= mask; 
	this->io60Chip->write_register(newResistor, this->resistors[offset][port]);
}

void io60p16::set_io_mode(unsigned char port, unsigned char pin, io_mode state, resistor_mode resistorMode)
{
	unsigned char mask = pin;
	unsigned char resistorRegister;

	if (state == io_modes::PWM_OUTPUT)	{
		if ((this->pwms[port] & mask) != 0x00)
			return; //already set as a PWM

		this->io60Chip->write_register(io60p16::PORT_SELECT_REGISTER, port);

		this->pwms[port] |= mask;

		this->io60Chip->write_register(io60p16::ENABLE_PWM_REGISTER, this->pwms[port]);
		
		this->write_digital(port, pin, true);

		this->io60Chip->write_register(io60p16::PWM_SELECT_REGISTER, (unsigned char)((pin % 8) + (port - 6) * 8));
		this->io60Chip->write_register(io60p16::PWM_CONFIG, io60p16::CLOCK_SOURCE); //93.75KHz clock
	}
	else {
		this->io60Chip->write_register(io60p16::PORT_SELECT_REGISTER, port);

		if ((this->pwms[port] & mask) != 0x00) {
			this->pwms[port] &= ~mask;

			this->io60Chip->write_register(io60p16::ENABLE_PWM_REGISTER, this->pwms[port]);
		}

		if (state == io_modes::DIGITAL_INPUT) {
			this->pinDirections[port] |= mask;

			resistorRegister = io60p16::PIN_HIGH_IMPEDENCE;
			
			if (resistorMode == resistor_modes::PULL_DOWN)
				resistorRegister = io60p16::PIN_PULL_DOWN;
			else if (resistorMode == resistor_modes::PULL_UP)
				resistorRegister = io60p16::PIN_PULL_UP;
		}
		else {
			this->pinDirections[port] &= ~mask;
			
			resistorRegister = io60p16::PIN_STRONG_DRIVE;
		}

		this->io60Chip->write_register(io60p16::PIN_DIRECTION_REGISTER, this->pinDirections[port]);
			
		this->changeResistor(port, mask, resistorRegister);
	}
}

//We're using the 93.75KHz clock source because it gives a good resolution around the 1KHz frequency
//while still allowing the user to select frequencies such as 10KHz, but with reduced duty cycle
//resolution.
void io60p16::set_pwm(unsigned char port, unsigned char pin, double duty_cycle, double frequency)
{
	this->io60Chip->write_register(io60p16::PWM_SELECT_REGISTER, (char)((pin % 8) + (port - 6) * 8));
	
	unsigned char period = (unsigned char)(93750 / frequency);

	this->io60Chip->write_register(io60p16::PERIOD_REGISTER, period);
	this->io60Chip->write_register(io60p16::PULSE_WIDTH_REGISTER, static_cast<unsigned char>(period * duty_cycle));
}

bool io60p16::read_digital(unsigned char port, unsigned char pin)
{
	unsigned char b = this->io60Chip->read_register(io60p16::INPUT_PORT_0_REGISTER + port);

	return (b & pin) != 0;
}

void io60p16::write_digital(unsigned char port, unsigned char pin, bool value)
{
	unsigned char mask = (1 << (pin & 0x0F));

	if (value)
		this->outputPorts[port] |= mask;
	else
		this->outputPorts[port] &= ~mask;
	
	this->io60Chip->write_register(io60p16::OUTPUT_PORT_0_REGISTER + port, this->outputPorts[port]);
}