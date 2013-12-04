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

io60p16::io60p16(unsigned char socket_number)
{
	const socket& s = mainboard->get_socket(socket_number, socket::types::X);

	this->chip = new devices::i2c(s.pins[5], s.pins[4], 0x20);

	for (int i = 0; i < 8; i++)
	{
		this->output_ports[i] = 0xFF;
		this->pin_directions[i] = 0x00;
		this->pwms[i] = 0x00;
		this->resistors[0][i] = 0xFF;
	}

	for (int i = 1; i < 4; i++)
		for (int j = 0; j < 8; j++)
			this->resistors[i][j] = 0x00;
}

io60p16::io60p16(const socket& sock, socket_pin_number sda, socket_pin_number scl)
{
	if (sda == 8 && scl == 9 && sock.i2c != i2c_channels::NONE)
		this->chip = new devices::i2c(sock.i2c, 0x20);
	else
		this->chip = new devices::i2c(sock.pins[sda], sock.pins[scl], 0x20);

	for (int i = 0; i < 8; i++)
	{
		this->output_ports[i] = 0xFF;
		this->pin_directions[i] = 0x00;
		this->pwms[i] = 0x00;
		this->resistors[0][i] = 0xFF;
	}

	for (int i = 1; i < 4; i++)
		for (int j = 0; j < 8; j++)
			this->resistors[i][j] = 0x00;
}

io60p16::~io60p16()
{
	delete this->chip;
}

void io60p16::change_resistor(unsigned char port, unsigned char mask, unsigned char new_resistor)
{
	for (unsigned int i = 0; i < 4; i++)
		this->resistors[i][port] &= ~mask;

	unsigned int offset = 0;
	switch (new_resistor)
	{
		case io60p16::PIN_PULL_UP: offset = 0; break;
		case io60p16::PIN_PULL_DOWN: offset = 1; break;
		case io60p16::PIN_HIGH_IMPEDENCE: offset = 2; break;
		case io60p16::PIN_STRONG_DRIVE: offset = 3; break;
	}

	this->resistors[offset][port] |= mask;
	this->chip->write_register(new_resistor, this->resistors[offset][port]);
}

void io60p16::set_io_mode(unsigned char port, unsigned char pin, io_mode new_io_mode, resistor_mode new_resistor_mode)
{
	unsigned char mask = 1 << pin;
	unsigned char resistor_register;

	this->chip->write_register(io60p16::PORT_SELECT_REGISTER, port);

	if ((this->pwms[port] & mask) != 0x00)
	{
		this->pwms[port] &= ~mask;

		this->chip->write_register(io60p16::ENABLE_PWM_REGISTER, this->pwms[port]);
	}

	if (new_io_mode == io_modes::DIGITAL_OUTPUT)
	{
		this->pin_directions[port] &= ~mask;

		resistor_register = io60p16::PIN_STRONG_DRIVE;
	}
	else
	{
		this->pin_directions[port] |= mask;

		if (new_resistor_mode == resistor_modes::PULL_DOWN)
			resistor_register = io60p16::PIN_PULL_DOWN;
		else if (new_resistor_mode == resistor_modes::PULL_UP)
			resistor_register = io60p16::PIN_PULL_UP;
		else
			resistor_register = io60p16::PIN_HIGH_IMPEDENCE;
	}

	this->chip->write_register(io60p16::PIN_DIRECTION_REGISTER, this->pin_directions[port]);

	this->change_resistor(port, mask, resistor_register);
}

void io60p16::set_pwm(unsigned char port, unsigned char pin, double frequency, double duty_cycle)
{
	unsigned char mask = 1 << pin;
	unsigned char pwm = static_cast<unsigned char>((pin % 8) + (port - 6) * 8);

	if ((this->pwms[port] & mask) == 0x00)
	{
		this->chip->write_register(io60p16::PORT_SELECT_REGISTER, port);

		this->pwms[port] |= mask;

		this->chip->write_register(io60p16::ENABLE_PWM_REGISTER, this->pwms[port]);

		this->write_digital(port, pin, true);

		this->chip->write_register(io60p16::PWM_SELECT_REGISTER, pwm);
		this->chip->write_register(io60p16::PWM_CONFIG, io60p16::CLOCK_SOURCE);
		this->change_resistor(port, mask, io60p16::PIN_STRONG_DRIVE);
	}

	this->chip->write_register(io60p16::PWM_SELECT_REGISTER, pwm);

	//We're using the 93.75KHz clock source because it gives a good resolution around the 1KHz frequency
	//while still allowing the user to select frequencies such as 10KHz, but with reduced duty cycle resolution.
	unsigned char period = static_cast<unsigned char>(93750 / frequency);

	this->chip->write_register(io60p16::PERIOD_REGISTER, period);
	this->chip->write_register(io60p16::PULSE_WIDTH_REGISTER, static_cast<unsigned char>(period * duty_cycle));
}

bool io60p16::read_digital(unsigned char port, unsigned char pin)
{
	unsigned char b = this->chip->read_register(io60p16::INPUT_PORT_0_REGISTER + port);

	return (b & (1 << pin)) != 0;
}

void io60p16::write_digital(unsigned char port, unsigned char pin, bool value)
{
	if (value)
		this->output_ports[port] |= (1 << pin);
	else
		this->output_ports[port] &= ~(1 << pin);

	this->chip->write_register(io60p16::OUTPUT_PORT_0_REGISTER + port, this->output_ports[port]);
}