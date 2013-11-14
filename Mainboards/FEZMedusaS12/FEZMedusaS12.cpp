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

#include "FEZMedusaS12.h"

using namespace gadgeteering;
using namespace gadgeteering::mainboards;

#define IS_EXTENDER_PIN(cpu_pin_number) ((cpu_pin_number & 0x80) != 0x00)
#define EXTENDER_PORT(cpu_pin_number) ((cpu_pin_number & 0x70) >> 4)
#define EXTENDER_PIN(cpu_pin_number) (cpu_pin_number & 0x0F)

fez_medusa_s12::fez_medusa_s12() : fez_medusa_mini()
{
	mainboard = this;

	this->create_sockets();

	this->extender = new modules::io60p16(3);
}

fez_medusa_s12::~fez_medusa_s12()
{
	delete this->extender;
}

void fez_medusa_s12::create_sockets()
{
	socket s5(5, socket::types::Y | socket::types::P | socket::types::X);
	s5.pins[3] = fez_medusa_s12::pins::P0_0;
	s5.pins[4] = fez_medusa_s12::pins::P0_1;
	s5.pins[5] = fez_medusa_s12::pins::P0_2;
	s5.pins[6] = fez_medusa_s12::pins::P0_3;
	s5.pins[7] = fez_medusa_s12::pins::P6_0;
	s5.pins[8] = fez_medusa_s12::pins::P6_1;
	s5.pins[9] = fez_medusa_s12::pins::P6_2;

	s5.pwm7 = pwm_channels::PWM_3;
	s5.pwm8 = pwm_channels::PWM_4;
	s5.pwm9 = pwm_channels::PWM_5;

	this->sockets[5] = s5;


	socket s6(6, socket::types::Y | socket::types::P | socket::types::X);
	s6.pins[3] = fez_medusa_s12::pins::P0_3;
	s6.pins[4] = fez_medusa_s12::pins::P0_4;
	s6.pins[5] = fez_medusa_s12::pins::P0_5;
	s6.pins[6] = fez_medusa_s12::pins::P0_6;
	s6.pins[7] = fez_medusa_s12::pins::P6_3;
	s6.pins[8] = fez_medusa_s12::pins::P6_4;
	s6.pins[9] = fez_medusa_s12::pins::P6_5;

	s6.pwm7 = pwm_channels::PWM_6;
	s6.pwm8 = pwm_channels::PWM_7;
	s6.pwm9 = pwm_channels::PWM_8;

	this->sockets[6] = s6;


	socket s7(7, socket::types::Y | socket::types::P | socket::types::X);
	s7.pins[3] = fez_medusa_s12::pins::P1_0;
	s7.pins[4] = fez_medusa_s12::pins::P1_1;
	s7.pins[5] = fez_medusa_s12::pins::P1_2;
	s7.pins[6] = fez_medusa_s12::pins::P1_3;
	s7.pins[7] = fez_medusa_s12::pins::P6_6;
	s7.pins[8] = fez_medusa_s12::pins::P6_7;
	s7.pins[9] = fez_medusa_s12::pins::P7_0;

	s7.pwm7 = pwm_channels::PWM_9;
	s7.pwm8 = pwm_channels::PWM_10;
	s7.pwm9 = pwm_channels::PWM_11;

	this->sockets[7] = s7;


	socket s8(8, socket::types::Y | socket::types::P | socket::types::X);
	s8.pins[3] = fez_medusa_s12::pins::P1_4;
	s8.pins[4] = fez_medusa_s12::pins::P1_5;
	s8.pins[5] = fez_medusa_s12::pins::P1_6;
	s8.pins[6] = fez_medusa_s12::pins::P1_7;
	s8.pins[7] = fez_medusa_s12::pins::P7_1;
	s8.pins[8] = fez_medusa_s12::pins::P7_2;
	s8.pins[9] = fez_medusa_s12::pins::P7_3;

	s8.pwm7 = pwm_channels::PWM_12;
	s8.pwm8 = pwm_channels::PWM_13;
	s8.pwm9 = pwm_channels::PWM_14;

	this->sockets[8] = s8;


	socket s9(9, socket::types::Y | socket::types::P | socket::types::X);
	s9.pins[3] = fez_medusa_s12::pins::P2_0;
	s9.pins[4] = fez_medusa_s12::pins::P2_1;
	s9.pins[5] = fez_medusa_s12::pins::P2_2;
	s9.pins[6] = fez_medusa_s12::pins::P2_3;
	s9.pins[7] = fez_medusa_s12::pins::P7_4;
	s9.pins[8] = fez_medusa_s12::pins::P7_5;
	s9.pins[9] = fez_medusa_s12::pins::P7_6;

	s9.pwm7 = pwm_channels::PWM_15;
	s9.pwm8 = pwm_channels::PWM_16;
	s9.pwm9 = pwm_channels::PWM_17;

	this->sockets[9] = s9;


	socket s10(10, socket::types::Y | socket::types::X);
	s10.pins[3] = fez_medusa_s12::pins::P3_0;
	s10.pins[4] = fez_medusa_s12::pins::P3_1;
	s10.pins[5] = fez_medusa_s12::pins::P3_2;
	s10.pins[6] = fez_medusa_s12::pins::P3_3;
	s10.pins[7] = fez_medusa_s12::pins::P3_4;
	s10.pins[8] = fez_medusa_s12::pins::P3_5;
	s10.pins[9] = fez_medusa_s12::pins::P3_6;
	this->sockets[10] = s10;


	socket s11(11, socket::types::Y | socket::types::X);
	s11.pins[3] = fez_medusa_s12::pins::P4_0;
	s11.pins[4] = fez_medusa_s12::pins::P4_1;
	s11.pins[5] = fez_medusa_s12::pins::P4_2;
	s11.pins[6] = fez_medusa_s12::pins::P4_3;
	s11.pins[7] = fez_medusa_s12::pins::P4_4;
	s11.pins[8] = fez_medusa_s12::pins::P4_5;
	s11.pins[9] = fez_medusa_s12::pins::P4_6;
	this->sockets[11] = s11;


	socket s12(12, socket::types::Y | socket::types::X);
	s12.pins[3] = fez_medusa_s12::pins::P5_0;
	s12.pins[4] = fez_medusa_s12::pins::P5_1;
	s12.pins[5] = fez_medusa_s12::pins::P5_2;
	s12.pins[6] = fez_medusa_s12::pins::P5_3;
	s12.pins[7] = fez_medusa_s12::pins::P5_4;
	s12.pins[8] = fez_medusa_s12::pins::P5_5;
	s12.pins[9] = fez_medusa_s12::pins::P5_6;
	this->sockets[12] = s12;
}

const socket& fez_medusa_s12::get_socket(unsigned char number)
{
	if (number == 0 || number == 4 || number > 12)
		system::panic(error_codes::INVALID_SOCKET);

	return this->sockets[number - 1];
}

void fez_medusa_s12::set_io_mode(cpu_pin pin, io_mode new_io_mode, resistor_mode new_resistor_mode)
{
	!IS_EXTENDER_PIN(pin) ? fez_medusa_mini::set_io_mode(pin, new_io_mode, new_resistor_mode) : this->extender->set_io_mode(EXTENDER_PORT(pin), EXTENDER_PIN(pin), new_io_mode, new_resistor_mode);
}

void fez_medusa_s12::set_pwm(pwm_channel channel, double duty_cycle, double frequency)
{
	cpu_pin pin;

	switch (channel)
	{
		case pwm_channels::PWM_0:
		case pwm_channels::PWM_1:
		case pwm_channels::PWM_2:
			fez_medusa_mini::set_pwm(channel, duty_cycle, frequency);
			break;

		case pwm_channels::PWM_3: pin = fez_medusa_s12::pins::P6_0; break;
		case pwm_channels::PWM_4: pin = fez_medusa_s12::pins::P6_1; break;
		case pwm_channels::PWM_5: pin = fez_medusa_s12::pins::P6_2; break;
		case pwm_channels::PWM_6: pin = fez_medusa_s12::pins::P6_3; break;
		case pwm_channels::PWM_7: pin = fez_medusa_s12::pins::P6_4; break;
		case pwm_channels::PWM_8: pin = fez_medusa_s12::pins::P6_5; break;
		case pwm_channels::PWM_9: pin = fez_medusa_s12::pins::P6_6; break;
		case pwm_channels::PWM_10: pin = fez_medusa_s12::pins::P6_7; break;
		case pwm_channels::PWM_11: pin = fez_medusa_s12::pins::P7_0; break;
		case pwm_channels::PWM_12: pin = fez_medusa_s12::pins::P7_1; break;
		case pwm_channels::PWM_13: pin = fez_medusa_s12::pins::P7_2; break;
		case pwm_channels::PWM_14: pin = fez_medusa_s12::pins::P7_3; break;
		case pwm_channels::PWM_15: pin = fez_medusa_s12::pins::P7_4; break;
		case pwm_channels::PWM_16: pin = fez_medusa_s12::pins::P7_5; break;
		case pwm_channels::PWM_17: pin = fez_medusa_s12::pins::P7_6; break;
	}

	this->extender->set_pwm(EXTENDER_PORT(pin), EXTENDER_PIN(pin), duty_cycle, frequency);
}

bool fez_medusa_s12::read_digital(cpu_pin pin)
{
	return !IS_EXTENDER_PIN(pin) ? fez_medusa_mini::read_digital(pin) : this->extender->read_digital(EXTENDER_PORT(pin), EXTENDER_PIN(pin));
}

void fez_medusa_s12::write_digital(cpu_pin pin, bool value)
{
	!IS_EXTENDER_PIN(pin) ? fez_medusa_mini::write_digital(pin, value) : this->extender->write_digital(EXTENDER_PORT(pin), EXTENDER_PIN(pin), value);
}
