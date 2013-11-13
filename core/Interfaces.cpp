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

#include "Interfaces.h"
#include "Mainboard.h"
#include "System.h"

using namespace gadgeteering;
using namespace gadgeteering::interfaces;

digital_output::digital_output(socket& socket, socket::pin pin_number, bool initial_state)
{
	if (socket.pins[pin_number] == socket::pins::UNCONNECTED)
		system::panic(Exceptions::ERR_PIN_UNCONNECTED);

	this->pin = socket.pins[pin_number];

	mainboard->set_io_mode(this->pin, io_modes::DIGITAL_OUTPUT, resistor_modes::FLOATING);

	this->write(initial_state);
}

void digital_output::write(bool value)
{
	mainboard->write_digital(this->pin, value);
}

digital_input::digital_input(socket& socket, socket::pin pin_number, resistor_mode new_resistor_mode)
{
	if (socket.pins[pin_number] == socket::pins::UNCONNECTED)
		system::panic(Exceptions::ERR_PIN_UNCONNECTED);

	this->pin = socket.pins[pin_number];
	this->set_resistor_mode(new_resistor_mode);
}

bool digital_input::read()
{
	return mainboard->read_digital(this->pin);
}

void digital_input::set_resistor_mode(resistor_mode new_resistor_mode)
{
	this->current_resistor_mode = new_resistor_mode;
	mainboard->set_io_mode(this->pin, io_modes::DIGITAL_INPUT, new_resistor_mode);
}

resistor_mode digital_input::get_resistor_mode()
{
	return this->current_resistor_mode;
}

digital_io::digital_io(socket& socket, socket::pin pin_number)
{
	if (socket.pins[pin_number] == socket::pins::UNCONNECTED)
		system::panic(Exceptions::ERR_PIN_UNCONNECTED);

	this->pin = socket.pins[pin_number];
	this->current_resistor_mode = resistor_modes::NONE;
	this->current_io_state = io_modes::NONE;
}

void digital_io::write(bool value)
{
	this->set_io_mode(io_modes::DIGITAL_OUTPUT);
	mainboard->write_digital(this->pin, value);
}

bool digital_io::read()
{
	this->set_io_mode(io_modes::DIGITAL_INPUT);
	return mainboard->read_digital(this->pin);
}

void digital_io::set_io_mode(io_mode new_io_mode)
{
	if (this->current_io_state == new_io_mode)
		return;

	this->current_io_state = new_io_mode;
	mainboard->set_io_mode(this->pin, this->current_io_state, this->current_resistor_mode);
}

void digital_io::set_resistor_mode(resistor_mode new_resistor_mode)
{
	if (this->current_resistor_mode == new_resistor_mode)
		return;

	this->current_resistor_mode = new_resistor_mode;
	mainboard->set_io_mode(this->pin, this->current_io_state, this->current_resistor_mode);
}

resistor_mode digital_io::get_resistor_mode()
{
	return this->current_resistor_mode;
}

io_mode digital_io::get_io_mode()
{
	return this->current_io_state;
}

analog_input::analog_input(socket& socket, socket::pin pin_number)
{
	switch (pin_number)
	{
		case socket::pins::THREE: this->channel = socket.analog1;
		case socket::pins::FOUR: this->channel = socket.analog2;
		case socket::pins::FIVE: this->channel = socket.analog3;
		default: system::panic(Exceptions::ERR_PIN_INVALID);
	}
}

double analog_input::read()
{
	return this->read_proportion() * mainboard->max_analog_voltage;
}

double analog_input::read_proportion()
{
	return mainboard->read_analog(this->channel);
}

analog_output::analog_output(socket& socket, socket::pin pin_number)
{
	switch (pin_number)
	{
		case socket::pins::THREE: this->channel = socket.analog1;
		case socket::pins::FOUR: this->channel = socket.analog2;
		case socket::pins::FIVE: this->channel = socket.analog3;
		default: system::panic(Exceptions::ERR_PIN_INVALID);
	}
}

void analog_output::write(double value)
{
	this->write_proportion(value / mainboard->max_analog_voltage);
}

void analog_output::write_proportion(double value)
{
	mainboard->write_analog(this->channel, value);
}

pwm_output::pwm_output(socket& socket, socket::pin pin_number)
{
	if (socket.pins[pin_number] == socket::pins::UNCONNECTED)
		system::panic(Exceptions::ERR_PIN_UNCONNECTED);

	this->pin = socket.pins[pin_number];

	mainboard->set_io_mode(this->pin, io_modes::PWM_OUTPUT, resistor_modes::FLOATING);

	this->set(0, 0);
}

void pwm_output::set(double frequency, double duty_cycle)
{
	this->duty_cycle = duty_cycle;
	this->frequency = frequency;

	mainboard->set_pwm(this->pin, this->frequency, this->duty_cycle);
}

void pwm_output::set_frequency(double frequency)
{
	this->set(frequency, this->duty_cycle);
}

void pwm_output::set_duty_cycle(double duty_cycle)
{
	this->set(this->frequency, duty_cycle);
}