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

#include "Socket.h"
#include "Types.h"

namespace gadgeteering 
{
	namespace interfaces 
	{
		class digital_output 
		{
			cpu_pin pin;

			public:
				digital_output(const socket& socket, socket::pin pin_number, bool initial_state = false);

				void write(bool value);
		};

		class digital_input {
			cpu_pin pin;
			resistor_mode current_resistor_mode;

			public:
				digital_input(const socket& socket, socket::pin pin_number, resistor_mode initial_resistor_mode = resistor_modes::FLOATING);

				bool read();

				void set_resistor_mode(resistor_mode new_resistor_mode);
				resistor_mode get_resistor_mode();
		};

		class digital_io {
			cpu_pin pin;
			io_mode current_io_state;
			resistor_mode current_resistor_mode;
	
			public:
				digital_io(const socket& socket, socket::pin pin_number);

				void write(bool value);
				bool read();

				void set_io_mode(io_mode new_io_mode);
				io_mode get_io_mode();

				void set_resistor_mode(resistor_mode new_resistor_mode);
				resistor_mode get_resistor_mode();
		};

		class analog_input {
			analog_channel channel;

			public:
				analog_input(const socket& socket, socket::pin pin_number);
				
				double read();
				double read_proportion();
		};

		class analog_output {
			analog_channel channel;

			public:
				analog_output(const socket& socket, socket::pin pin_number);
				
				void write(double value);
				void write_proportion(double value);
		};

		class pwm_output {
			cpu_pin pin;

			double frequency;
			double duty_cycle;

			public:
				pwm_output(const socket& socket, socket::pin pin_number);

				void set(double frequency, double duty_cycle);
				void set_frequency(double frequency);
				void set_duty_cycle(double duty_cycle);
		};
	}
}
