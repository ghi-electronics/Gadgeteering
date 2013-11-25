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

#include "PulseInOut.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

pulse_in_out::pulse_in_out(unsigned char socket_number) : daisy_link::module(socket_number, pulse_in_out::GHI_DAISYLINK_MANUFACTURER, pulse_in_out::GHI_DAISYLINK_TYPE_PULSE, pulse_in_out::GHI_DAISYLINK_VERSION_PULSE, pulse_in_out::GHI_DAISYLINK_VERSION_PULSE)
{

}

void pulse_in_out::write_register(unsigned char address, unsigned char value)
{
	daisy_link::module::write_register(daisy_link::module::REGISTER_OFFSET + address, value);
}

unsigned char pulse_in_out::read_register(unsigned char address)
{
	return daisy_link::module::read_register(address);
}

void pulse_in_out::read_channel(int input_id, int& high_time, int& low_time)
{
	unsigned char reg= (pulse_in_out::REGISTER_OFFSET + (input_id - 1) * 2);
	high_time = static_cast<unsigned short>(this->read_register(reg) | (this->read_register((reg + 1)) << 8));
	low_time = static_cast<unsigned short>(this->read_register((reg + 16)) | (this->read_register((reg + 16 + 1)) << 8));

	high_time *= 10;
	low_time *= 10;
}

void pulse_in_out::set_pulse(int pwm_id, unsigned long period_micro, unsigned long high_time_micro)
{
	unsigned char register_period = static_cast<unsigned char>(pulse_in_out::REGISTER_PERIOD_PWM012_FREQUENCY - pulse_in_out::REGISTER_OFFSET);
	unsigned char register_high_time = static_cast<unsigned char>(pulse_in_out::REGISTER_PWM_PULSE - pulse_in_out::REGISTER_OFFSET + (pwm_id - 1) * 4);

	unsigned char d1[5] = { static_cast<unsigned char>(register_period + 0 + daisy_link::module::REGISTER_OFFSET), static_cast<unsigned char>(period_micro),
							static_cast<unsigned char>(period_micro >> 8),
							static_cast<unsigned char>(period_micro >> 16),
							static_cast<unsigned char>(period_micro >> 24) };
	unsigned char d2[5] = { static_cast<unsigned char>(register_high_time + 0 + daisy_link::module::REGISTER_OFFSET), static_cast<unsigned char>(high_time_micro),
							static_cast<unsigned char>(high_time_micro >> 8),
							static_cast<unsigned char>(period_micro >> 16),
							static_cast<unsigned char>(period_micro >> 24) };

	this->write(d1, 5);
	this->write(d2, 5);
}

void pulse_in_out::set_pulse(int pwm_id, unsigned long high_time_micro)
{
	unsigned char reg = static_cast<unsigned char>(pulse_in_out::REGISTER_PWM_PULSE - pulse_in_out::REGISTER_OFFSET + (pwm_id - 1) * 4);

	unsigned char d1[5] = { static_cast<unsigned char>(reg + 0 + daisy_link::module::REGISTER_OFFSET), static_cast<unsigned char>(high_time_micro),
							static_cast<unsigned char>(high_time_micro >> 8),
							static_cast<unsigned char>(high_time_micro >> 16),
							static_cast<unsigned char>(high_time_micro >> 24) };

	this->write(d1, 5);
}
