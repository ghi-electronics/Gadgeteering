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

#include "HubAP5.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;

#define GET_PORT(cpu_pin_number) ((cpu_pin_number & 0x70) >> 4)
#define GET_PIN(cpu_pin_number) (cpu_pin_number & 0x0F)

hubap5::indirected_digital_input::indirected_digital_input(const socket& sock, hubap5& hub) : sock(sock), hub(hub)
{

}

bool hubap5::indirected_digital_input::read(socket::pin pin_number)
{
	return this->hub.read_digital(this->sock.pins[pin_number]);
}

void hubap5::indirected_digital_input::set_input(socket::pin pin_number, resistor_mode mode)
{
	this->hub.set_io_mode(this->sock.pins[pin_number], io_modes::DIGITAL_INPUT, mode);
}

hubap5::indirected_digital_output::indirected_digital_output(const socket& sock, hubap5& hub) : sock(sock), hub(hub)
{

}

void hubap5::indirected_digital_output::write(socket::pin pin_number, bool value)
{
	this->hub.write_digital(this->sock.pins[pin_number], value);
}

void hubap5::indirected_digital_output::set_output(socket::pin pin_number)
{
	this->hub.set_io_mode(this->sock.pins[pin_number], io_modes::DIGITAL_OUTPUT, resistor_modes::NONE);
}

hubap5::indirected_digital_io::indirected_digital_io(const socket& sock, hubap5& hub) : sock(sock), hub(hub)
{

}

void hubap5::indirected_digital_io::write(socket::pin pin_number, bool value)
{
	this->hub.write_digital(this->sock.pins[pin_number], value);
}

bool hubap5::indirected_digital_io::read(socket::pin pin_number)
{
	return this->hub.read_digital(this->sock.pins[pin_number]);
}

void hubap5::indirected_digital_io::set_io_mode(socket::pin pin_number, io_mode new_io_mode, resistor_mode new_resistor_mode)
{
	this->hub.set_io_mode(this->sock.pins[pin_number], new_io_mode, new_resistor_mode);
}

hubap5::indirected_analog_input::indirected_analog_input(const socket& sock, hubap5& hub) : sock(sock), hub(hub)
{

}

double hubap5::indirected_analog_input::read(socket::pin pin_number)
{
	return this->hub.read_analog(this->sock.pins[pin_number]);
}

hubap5::indirected_pwm_output::indirected_pwm_output(const socket& sock, hubap5& hub) : sock(sock), hub(hub)
{

}

void hubap5::indirected_pwm_output::set(socket::pin pin_number, double duty_cycle, double frequency)
{
	this->hub.set_pwm(this->sock.pins[pin_number], duty_cycle, frequency);
}

hubap5::hubap5(unsigned char socket_number) : socket_start(socket_number * 10), socket_1(this->socket_start + 1), socket_2(this->socket_start + 2), socket_3(this->socket_start + 3), socket_4(this->socket_start + 4), socket_5(this->socket_start + 5), socket_6(this->socket_start + 6), socket_7(this->socket_start + 7), socket_8(this->socket_start + 8)
{
	const socket& s = mainboard->get_socket(socket_number);

	s.ensure_type(socket::types::I);

	this->io60_chip = new io60p16(s);
	this->analog_chip = new devices::i2c(s.i2c, 0x48);

	this->create_sockets();
}

hubap5::~hubap5()
{
	delete this->io60_chip;
	delete this->analog_chip;
}

void hubap5::create_sockets()
{
	socket s1(this->socket_start + 1, socket::types::Y | socket::types::X | socket::types::A);
	s1.pins[3] = hubap5::pins::P5_0;
	s1.pins[4] = hubap5::pins::P5_1;
	s1.pins[5] = hubap5::pins::P5_2;
	s1.pins[6] = hubap5::pins::P5_3;
	s1.pins[7] = hubap5::pins::P5_4;
	s1.pins[8] = hubap5::pins::P5_5;
	s1.pins[9] = hubap5::pins::P5_6;

	s1.analog3 = analog_channels::ANALOG_0;
	s1.analog4 = analog_channels::ANALOG_1;
	s1.analog5 = analog_channels::ANALOG_2;

	socket& ns1 = mainboard->register_socket(s1);
	ns1.analog_input_indirector = new indirected_analog_input(ns1, *this);
	ns1.digital_input_indirector = new indirected_digital_input(ns1, *this);
	ns1.digital_output_indirector = new indirected_digital_output(ns1, *this);
	ns1.digital_io_indirector = new indirected_digital_io(ns1, *this);


	socket s2(this->socket_start + 2, socket::types::Y | socket::types::X | socket::types::A);
	s2.pins[3] = hubap5::pins::P4_0;
	s2.pins[4] = hubap5::pins::P4_1;
	s2.pins[5] = hubap5::pins::P4_2;
	s2.pins[6] = hubap5::pins::P4_3;
	s2.pins[7] = hubap5::pins::P4_4;
	s2.pins[8] = hubap5::pins::P4_5;
	s2.pins[9] = hubap5::pins::P4_6;

	s2.analog3 = analog_channels::ANALOG_3;
	s2.analog4 = analog_channels::ANALOG_4;
	s2.analog5 = analog_channels::ANALOG_5;

	socket& ns2 = mainboard->register_socket(s2);
	ns2.analog_input_indirector = new indirected_analog_input(ns2, *this);
	ns2.digital_input_indirector = new indirected_digital_input(ns2, *this);
	ns2.digital_output_indirector = new indirected_digital_output(ns2, *this);
	ns2.digital_io_indirector = new indirected_digital_io(ns2, *this);


	socket s3(this->socket_start + 3, socket::types::Y | socket::types::X);
	s3.pins[3] = hubap5::pins::P3_0;
	s3.pins[4] = hubap5::pins::P3_1;
	s3.pins[5] = hubap5::pins::P3_2;
	s3.pins[6] = hubap5::pins::P3_3;
	s3.pins[7] = hubap5::pins::P3_4;
	s3.pins[8] = hubap5::pins::P3_5;
	s3.pins[9] = hubap5::pins::P3_6;

	socket& ns3 = mainboard->register_socket(s3);
	ns3.analog_input_indirector = new indirected_analog_input(ns3, *this);
	ns3.digital_input_indirector = new indirected_digital_input(ns3, *this);
	ns3.digital_output_indirector = new indirected_digital_output(ns3, *this);
	ns3.digital_io_indirector = new indirected_digital_io(ns3, *this);


	socket s4(this->socket_start + 4, socket::types::Y | socket::types::P | socket::types::X);
	s4.pins[3] = hubap5::pins::P2_0;
	s4.pins[4] = hubap5::pins::P2_1;
	s4.pins[5] = hubap5::pins::P2_2;
	s4.pins[6] = hubap5::pins::P2_3;
	s4.pins[7] = hubap5::pins::P7_4;
	s4.pins[8] = hubap5::pins::P7_5;
	s4.pins[9] = hubap5::pins::P7_6;

	s4.pwm7 = pwm_channels::PWM_12;
	s4.pwm8 = pwm_channels::PWM_13;
	s4.pwm9 = pwm_channels::PWM_14;

	socket& ns4 = mainboard->register_socket(s4);
	ns4.analog_input_indirector = new indirected_analog_input(ns4, *this);
	ns4.digital_input_indirector = new indirected_digital_input(ns4, *this);
	ns4.digital_output_indirector = new indirected_digital_output(ns4, *this);
	ns4.digital_io_indirector = new indirected_digital_io(ns4, *this);
	ns4.pwm_output_indirector = new indirected_pwm_output(ns4, *this);


	socket s5(this->socket_start + 5, socket::types::Y | socket::types::P | socket::types::X);
	s5.pins[3] = hubap5::pins::P1_4;
	s5.pins[4] = hubap5::pins::P1_5;
	s5.pins[5] = hubap5::pins::P1_6;
	s5.pins[6] = hubap5::pins::P1_7;
	s5.pins[7] = hubap5::pins::P7_1;
	s5.pins[8] = hubap5::pins::P7_2;
	s5.pins[9] = hubap5::pins::P7_3;

	s5.pwm7 = pwm_channels::PWM_9;
	s5.pwm8 = pwm_channels::PWM_10;
	s5.pwm9 = pwm_channels::PWM_11;

	socket& ns5 = mainboard->register_socket(s5);
	ns5.analog_input_indirector = new indirected_analog_input(ns5, *this);
	ns5.digital_input_indirector = new indirected_digital_input(ns5, *this);
	ns5.digital_output_indirector = new indirected_digital_output(ns5, *this);
	ns5.digital_io_indirector = new indirected_digital_io(ns5, *this);
	ns5.pwm_output_indirector = new indirected_pwm_output(ns5, *this);


	socket s6(this->socket_start + 6, socket::types::Y | socket::types::P | socket::types::X);
	s6.pins[3] = hubap5::pins::P1_0;
	s6.pins[4] = hubap5::pins::P1_1;
	s6.pins[5] = hubap5::pins::P1_2;
	s6.pins[6] = hubap5::pins::P1_3;
	s6.pins[7] = hubap5::pins::P6_6;
	s6.pins[8] = hubap5::pins::P6_7;
	s6.pins[9] = hubap5::pins::P7_0;

	s6.pwm7 = pwm_channels::PWM_6;
	s6.pwm8 = pwm_channels::PWM_7;
	s6.pwm9 = pwm_channels::PWM_8;

	socket& ns6 = mainboard->register_socket(s6);
	ns6.analog_input_indirector = new indirected_analog_input(ns6, *this);
	ns6.digital_input_indirector = new indirected_digital_input(ns6, *this);
	ns6.digital_output_indirector = new indirected_digital_output(ns6, *this);
	ns6.digital_io_indirector = new indirected_digital_io(ns6, *this);
	ns6.pwm_output_indirector = new indirected_pwm_output(ns6, *this);


	socket s7(this->socket_start + 7, socket::types::Y | socket::types::P | socket::types::X);
	s6.pins[3] = hubap5::pins::P0_4;
	s6.pins[4] = hubap5::pins::P0_5;
	s6.pins[5] = hubap5::pins::P0_6;
	s6.pins[6] = hubap5::pins::P0_7;
	s6.pins[7] = hubap5::pins::P6_3;
	s6.pins[8] = hubap5::pins::P6_4;
	s6.pins[9] = hubap5::pins::P6_5;

	s7.pwm7 = pwm_channels::PWM_3;
	s7.pwm8 = pwm_channels::PWM_4;
	s7.pwm9 = pwm_channels::PWM_5;

	socket& ns7 = mainboard->register_socket(s7);
	ns7.analog_input_indirector = new indirected_analog_input(ns7, *this);
	ns7.digital_input_indirector = new indirected_digital_input(ns7, *this);
	ns7.digital_output_indirector = new indirected_digital_output(ns7, *this);
	ns7.digital_io_indirector = new indirected_digital_io(ns7, *this);
	ns7.pwm_output_indirector = new indirected_pwm_output(ns7, *this);


	socket s8(this->socket_start + 8, socket::types::Y | socket::types::P | socket::types::X);
	s8.pins[3] = hubap5::pins::P0_0;
	s8.pins[4] = hubap5::pins::P0_1;
	s8.pins[5] = hubap5::pins::P0_2;
	s8.pins[6] = hubap5::pins::P0_3;
	s8.pins[7] = hubap5::pins::P6_0;
	s8.pins[8] = hubap5::pins::P6_1;
	s8.pins[9] = hubap5::pins::P6_2;

	s8.pwm7 = pwm_channels::PWM_0;
	s8.pwm8 = pwm_channels::PWM_1;
	s8.pwm9 = pwm_channels::PWM_2;

	socket& ns8 = mainboard->register_socket(s8);
	ns8.analog_input_indirector = new indirected_analog_input(ns8, *this);
	ns8.digital_input_indirector = new indirected_digital_input(ns8, *this);
	ns8.digital_output_indirector = new indirected_digital_output(ns8, *this);
	ns8.digital_io_indirector = new indirected_digital_io(ns8, *this);
	ns8.pwm_output_indirector = new indirected_pwm_output(ns8, *this);
}

void hubap5::set_io_mode(cpu_pin pin, io_mode new_io_mode, resistor_mode new_resistor_mode)
{
	this->io60_chip->set_io_mode(GET_PORT(pin), GET_PIN(pin), new_io_mode, new_resistor_mode);
}

void hubap5::set_pwm(pwm_channel channel, double duty_cycle, double frequency)
{
	if (channel == pwm_channels::NONE || channel > pwm_channels::PWM_14)
		system::panic(error_codes::CHANNEL_INVALID);

	unsigned char pin = 0xFF;

	switch (channel)
	{
		case pwm_channels::PWM_0: pin = hubap5::pins::P6_0; break;
		case pwm_channels::PWM_1: pin = hubap5::pins::P6_1; break;
		case pwm_channels::PWM_2: pin = hubap5::pins::P6_2; break;
		case pwm_channels::PWM_3: pin = hubap5::pins::P6_3; break;
		case pwm_channels::PWM_4: pin = hubap5::pins::P6_4; break;
		case pwm_channels::PWM_5: pin = hubap5::pins::P6_5; break;
		case pwm_channels::PWM_6: pin = hubap5::pins::P6_6; break;
		case pwm_channels::PWM_7: pin = hubap5::pins::P6_7; break;
		case pwm_channels::PWM_8: pin = hubap5::pins::P7_0; break;
		case pwm_channels::PWM_9: pin = hubap5::pins::P7_1; break;
		case pwm_channels::PWM_10: pin = hubap5::pins::P7_2; break;
		case pwm_channels::PWM_11: pin = hubap5::pins::P7_3; break;
		case pwm_channels::PWM_12: pin = hubap5::pins::P7_4; break;
		case pwm_channels::PWM_13: pin = hubap5::pins::P7_5; break;
		case pwm_channels::PWM_14: pin = hubap5::pins::P7_6; break;
	}

	this->io60_chip->set_pwm(GET_PORT(pin), GET_PIN(pin), duty_cycle, frequency);
}

bool hubap5::read_digital(cpu_pin pin)
{
	return this->io60_chip->read_digital(GET_PORT(pin), GET_PIN(pin));
}

void hubap5::write_digital(cpu_pin pin, bool value)
{
	this->io60_chip->write_digital(GET_PORT(pin), GET_PIN(pin), value);
}

double hubap5::read_analog(analog_channel channel)
{
	if (channel == analog_channels::NONE || channel > analog_channels::ANALOG_5)
		system::panic(error_codes::CHANNEL_INVALID);

	unsigned char pin = 0xFF;

	switch (channel)
	{
		case analog_channels::ANALOG_0: pin = hubap5::pins::P5_0; break;
		case analog_channels::ANALOG_1: pin = hubap5::pins::P5_1; break;
		case analog_channels::ANALOG_2: pin = hubap5::pins::P5_2; break;
		case analog_channels::ANALOG_3: pin = hubap5::pins::P4_0; break;
		case analog_channels::ANALOG_4: pin = hubap5::pins::P4_1; break;
		case analog_channels::ANALOG_5: pin = hubap5::pins::P4_2; break;
	}

	this->set_io_mode(pin, io_modes::DIGITAL_INPUT, resistor_modes::FLOATING);

	unsigned char read, command = 0x80 | 0x04; //CMD_SD_SE | CMD_PD_ON

	command |= static_cast<unsigned char>((channel % 2 == 0 ? channel / 2 : (channel - 1) / 2 + 4) << 4);

	this->analog_chip->write_read(&command, 1, &read, 1);

	return static_cast<double>(read) / 255.0;
}