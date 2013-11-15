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

digital_output::digital_output(const socket& socket, socket_pin_number pin_number, bool initial_state) : sock(socket), sock_pin(pin_number), pin(sock.pins[pin_number])
{
	if (socket.pins[pin_number] == UNCONNECTED_PIN)
		panic(errors::SOCKET_PIN_NOT_CONNECTED);

	if (!this->sock.digital_output_indirector)
	{
		mainboard->set_io_mode(this->pin, io_modes::DIGITAL_OUTPUT, resistor_modes::FLOATING);
	}
	else
	{
		this->sock.digital_output_indirector->set_output(this->sock_pin);
	}
	
	this->write(initial_state);
}

void digital_output::write(bool value)
{
	if (!this->sock.digital_output_indirector)
	{
		mainboard->write_digital(this->pin, value);
	}
	else
	{
		this->sock.digital_output_indirector->write(this->sock_pin, value);
	}
}

digital_input::digital_input(const socket& socket, socket_pin_number pin_number, resistor_mode new_resistor_mode) : sock(socket), sock_pin(pin_number), pin(sock.pins[pin_number])
{
	if (socket.pins[pin_number] == UNCONNECTED_PIN)
		panic(errors::SOCKET_PIN_NOT_CONNECTED);

	if (!this->sock.digital_input_indirector)
	{
		mainboard->set_io_mode(this->pin, io_modes::DIGITAL_OUTPUT, resistor_modes::FLOATING);
	}
	else
	{
		this->sock.digital_input_indirector->set_input(this->sock_pin, new_resistor_mode);
	}
	
	this->set_resistor_mode(new_resistor_mode);
}

bool digital_input::read()
{
	if (!this->sock.digital_input_indirector)
	{
		return mainboard->read_digital(this->pin);
	}
	else
	{
		return this->sock.digital_input_indirector->read(this->sock_pin);
	}
}

void digital_input::set_resistor_mode(resistor_mode new_resistor_mode)
{
	this->current_resistor_mode = new_resistor_mode;

	if (!this->sock.digital_input_indirector)
	{
		mainboard->set_io_mode(this->pin, io_modes::DIGITAL_INPUT, new_resistor_mode);
	}
	else
	{
		this->sock.digital_input_indirector->set_input(this->sock_pin, new_resistor_mode);
	}
}

resistor_mode digital_input::get_resistor_mode()
{
	return this->current_resistor_mode;
}

digital_io::digital_io(const socket& socket, socket_pin_number pin_number) : sock(socket), sock_pin(pin_number), pin(sock.pins[pin_number])
{
	if (socket.pins[pin_number] == UNCONNECTED_PIN)
		panic(errors::SOCKET_PIN_NOT_CONNECTED);

	this->current_resistor_mode = resistor_modes::NONE;
	this->current_io_state = io_modes::NONE;
}

void digital_io::write(bool value)
{
	this->set_io_mode(io_modes::DIGITAL_OUTPUT);

	if (!this->sock.digital_io_indirector)
	{
		mainboard->write_digital(this->pin, value);
	}
	else
	{
		this->sock.digital_io_indirector->write(this->sock_pin, value);
	}
}

bool digital_io::read()
{
	this->set_io_mode(io_modes::DIGITAL_INPUT);

	if (!this->sock.digital_io_indirector)
	{
		return mainboard->read_digital(this->pin);
	}
	else
	{
		return this->sock.digital_io_indirector->read(this->sock_pin);
	}
}

void digital_io::set_io_mode(io_mode new_io_mode)
{
	if (this->current_io_state == new_io_mode)
		return;

	this->current_io_state = new_io_mode;

	if (!this->sock.digital_io_indirector)
	{
		mainboard->set_io_mode(this->pin, this->current_io_state, this->current_resistor_mode);
	}
	else
	{
		this->sock.digital_io_indirector->set_io_mode(this->sock_pin, this->current_io_state, this->current_resistor_mode);
	}
}

void digital_io::set_resistor_mode(resistor_mode new_resistor_mode)
{
	if (this->current_resistor_mode == new_resistor_mode)
		return;

	this->current_resistor_mode = new_resistor_mode;

	if (!this->sock.digital_io_indirector)
	{
		mainboard->set_io_mode(this->pin, this->current_io_state, this->current_resistor_mode);
	}
	else
	{
		this->sock.digital_io_indirector->set_io_mode(this->sock_pin, this->current_io_state, this->current_resistor_mode);
	}
}

resistor_mode digital_io::get_resistor_mode()
{
	return this->current_resistor_mode;
}

io_mode digital_io::get_io_mode()
{
	return this->current_io_state;
}

analog_input::analog_input(const socket& socket, socket_pin_number pin_number) : sock(socket), sock_pin(pin_number), pin(sock.pins[pin_number])
{
	if (!this->sock.analog_input_indirector)
	{
		switch (pin_number)
		{
			case 3: this->channel = socket.analog3;
			case 4: this->channel = socket.analog4;
			case 5: this->channel = socket.analog5;
		}

		if (this->channel == analog_channels::NONE)
			panic(errors::PIN_DOES_NOT_SUPPORT_THIS_TYPE);
	}
	else
	{
		if (socket.pins[pin_number] == UNCONNECTED_PIN)
			panic(errors::PIN_DOES_NOT_SUPPORT_THIS_TYPE);
	}
}

double analog_input::read()
{
	return this->read_proportion() * mainboard->max_analog_voltage;
}

double analog_input::read_proportion()
{
	if (!this->sock.analog_input_indirector)
	{
		return mainboard->read_analog(this->channel);
	}
	else
	{
		return this->sock.analog_input_indirector->read(this->sock_pin);
	}
}

analog_output::analog_output(const socket& socket, socket_pin_number pin_number) : sock(socket), sock_pin(pin_number), pin(sock.pins[pin_number])
{
	if (!this->sock.analog_output_indirector)
	{
		if (pin_number != 5 || socket.analog_out == analog_out_channels::NONE)
			panic(errors::PIN_DOES_NOT_SUPPORT_THIS_TYPE);

		this->channel = socket.analog_out;
	}
	else
	{
		if (pin_number != 5 || socket.pins[pin_number] == UNCONNECTED_PIN)
			panic(errors::PIN_DOES_NOT_SUPPORT_THIS_TYPE);
	}
}

void analog_output::write(double value)
{
	this->write_proportion(value / mainboard->max_analog_voltage);
}

void analog_output::write_proportion(double value)
{
	if (!this->sock.analog_output_indirector)
	{
		mainboard->write_analog(this->channel, value);
	}
	else
	{
		this->sock.analog_output_indirector->write(this->sock_pin, value);
	}
}

pwm_output::pwm_output(const socket& socket, socket_pin_number pin_number) : sock(socket), sock_pin(pin_number), pin(sock.pins[pin_number])
{
	if (!this->sock.pwm_output_indirector)
	{
		switch (pin_number)
		{
			case 7: this->channel = socket.pwm7;
			case 8: this->channel = socket.pwm8;
			case 9: this->channel = socket.pwm9;
		}

		if (this->channel == pwm_channels::NONE)
			panic(errors::PIN_DOES_NOT_SUPPORT_THIS_TYPE);
	}
	else
	{
		if (socket.pins[pin_number] == UNCONNECTED_PIN)
			panic(errors::PIN_DOES_NOT_SUPPORT_THIS_TYPE);
	}

	this->set(0, 0);
}

void pwm_output::set(double frequency, double duty_cycle)
{
	this->duty_cycle = duty_cycle;
	this->frequency = frequency;

	if (!this->sock.pwm_output_indirector)
	{
		mainboard->set_pwm(this->channel, this->frequency, this->duty_cycle);
	}
	else
	{
		this->sock.pwm_output_indirector->set(this->sock_pin, duty_cycle, frequency);
	}
}

void pwm_output::set_frequency(double frequency)
{
	this->set(frequency, this->duty_cycle);
}

void pwm_output::set_duty_cycle(double duty_cycle)
{
	this->set(this->frequency, duty_cycle);
}