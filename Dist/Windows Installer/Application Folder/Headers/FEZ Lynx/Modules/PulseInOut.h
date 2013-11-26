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

#pragma once

#include "../Gadgeteering.h"

namespace gadgeteering
{
	namespace modules
	{
		class pulse_in_out : public daisy_link::module
		{
			static const unsigned char GHI_DAISYLINK_MANUFACTURER = 0x10;
			static const unsigned char GHI_DAISYLINK_TYPE_PULSE = 0x06;
			static const unsigned char GHI_DAISYLINK_VERSION_PULSE = 0x01;

			static const int REGISTER_OFFSET = 8;
			static const int REGISTER_CHANNEL_HIGH = REGISTER_OFFSET;
			static const int REGISTER_CHANNEL_LOW = REGISTER_CHANNEL_HIGH + 2 * 8;
			static const int REGISTER_PERIOD_PWM012_FREQUENCY = REGISTER_CHANNEL_LOW + 2 * 8;
			 
			static const int REGISTER_PERIOD_PWM345_FREQUENCY = REGISTER_PERIOD_PWM012_FREQUENCY + 4 * 1;
			static const int REGISTER_PERIOD_PWM67_FREQUENCY = REGISTER_PERIOD_PWM345_FREQUENCY + 4 * 1;
			static const int REGISTER_PWM_PULSE = REGISTER_PERIOD_PWM67_FREQUENCY + 4 * 1;

			void write_register(unsigned char address, unsigned char value);
			unsigned char read_register(unsigned char address);

			public:
				pulse_in_out(unsigned char socket_number);

				void read_channel(int input_id, int& high_time, int& low_time);
				void set_pulse(int pwm_id, unsigned long period_micro, unsigned long high_time_micro);
				void set_pulse(int pwm_id, unsigned long high_time_micro);
		};
	}
}
